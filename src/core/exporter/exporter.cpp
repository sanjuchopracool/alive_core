#include "exporter.h"
#include <core/composition/composition_node.h>
#include <core/model/composition.h>
#include <thread>

#include "include/core/SkCanvas.h"
#include "include/core/SkData.h"
#include "include/core/SkImage.h"
#include "include/core/SkStream.h"
#include "include/core/SkSurface.h"
#include "include/encode/SkPngEncoder.h"
#include "include/encode/SkWebpEncoder.h"
#include <atomic>
#include <condition_variable>
#include <fmt/format.h>
#include <mutex>
#include <third_party/externals/libwebp/src/webp/encode.h>
#include <third_party/externals/libwebp/src/webp/mux.h>

namespace inae::exporter {

class IEncoderInterface
{
public:
    virtual void init() {}
    virtual void write_frame(SkCanvas *canvas = nullptr) = 0;
    virtual void flush() {}
};

std::mutex k_encode_mutex;
std::atomic_int k_frame_index = 0;
std::atomic_int k_next_flushed_index = 0;
std::condition_variable k_cv;

int get_new_counter()
{
    return k_frame_index++;
}

Exporter::Exporter(const Composition *comp)
    : m_comp(comp)
{}

struct ThreadContext
{
    ThreadContext(const Composition *comp,
                  const std::string &path,
                  Exporter::Format format = Exporter::e_PNGSequence,
                  WebPAnimEncoder *encoder = nullptr,
                  const WebPConfig *config = nullptr)
        : m_comp(comp)
        , m_container(std::make_unique<CompositionNode>(m_comp))
        , m_path(path)
        , m_format(format)
        , m_encoder(encoder)
        , m_config(config)
    {
        if (m_format == Exporter::e_WebM && (!encoder || !config || !WebPPictureInit(&m_pic))) {
            throw "ERROR!";
        }
    }

    void operator()()
    {
        const float m_s_per_frame = 1000 / m_comp->framerate();
        sk_sp<SkSurface> raster_surface = SkSurfaces::Raster(
            SkImageInfo::MakeN32Premul(m_comp->width(), m_comp->height()));
        SkCanvas *raster_canvas = raster_surface->getCanvas();
        int i = 0;
        while ((i = get_new_counter()) <= m_comp->out_point()) {
            m_container->update(i, true);
            raster_canvas->clear(SK_ColorTRANSPARENT);
            m_container->draw(raster_canvas);
            // export as png
            if (m_format == Exporter::e_PNGSequence) {
                sk_sp<SkImage> img(raster_surface->makeImageSnapshot());
                if (!img) {
                    return;
                }

                sk_sp<SkData> png = SkPngEncoder::Encode(nullptr,
                                                         img.get(),
                                                         SkPngEncoder::Options());
                if (!png) {
                    return;
                }

                std::string path = m_path + fmt::format("/{}{:04}.png", m_comp->name(), i);
                SkFILEWStream out(path.c_str());
                (void) out.write(png->data(), png->size());
            } else if (m_format == Exporter::e_WebM) {
                SkImageInfo info;
                size_t row_bytes;
                void *base_ptr = raster_canvas->accessTopLayerPixels(&info, &row_bytes, nullptr);
                if (!base_ptr) {
                    throw "Invalid Pointer";
                }

                m_pic.width = info.width();
                m_pic.height = info.height();
                int status = 0;
                if (info.colorType() == SkColorType::kRGBA_8888_SkColorType) {
                    status = WebPPictureImportRGBA(&m_pic,
                                                   static_cast<uint8_t *>(base_ptr),
                                                   info.width() * 4);
                } else if (info.colorType() == SkColorType::kBGRA_8888_SkColorType) {
                    status = WebPPictureImportBGRA(&m_pic,
                                                   static_cast<uint8_t *>(base_ptr),
                                                   row_bytes);
                } else {
                    throw "Unsupported format!";
                }
                if (!status) {
                    throw "Invalid frame";
                }

                {
                    std::unique_lock lk(k_encode_mutex);
                    k_cv.wait(lk, [&i] { return i == k_next_flushed_index; });
                    bool ok = WebPAnimEncoderAdd(m_encoder,
                                                 &m_pic,
                                                 static_cast<int>(i * m_s_per_frame),
                                                 m_config);
                    if (!ok) {
                        throw;
                    }
                    k_next_flushed_index++;
                    lk.unlock();
                }
                k_cv.notify_all();

                WebPPictureFree(&m_pic);
            }
        }
    }

private:
    const Composition *m_comp = nullptr;
    std::unique_ptr<CompositionNode> m_container;
    std::string m_path;
    Exporter::Format m_format;
    WebPPicture m_pic;
    WebPAnimEncoder *m_encoder = nullptr;
    const WebPConfig *m_config = nullptr;
};

class ScopedThread
{
public:
    ScopedThread(ThreadContext &tc)
        : m_t(std::ref(tc))
    {
        m_t.join();
    }
    ~ScopedThread() = default;
    ScopedThread(const ScopedThread &other) = delete;
    ScopedThread &operator=(const ScopedThread &other) = delete;

private:
    std::thread m_t;
};

bool Exporter::export_as(const std::string &path, Format format)
{
    try {
        k_frame_index = 0;
        k_next_flushed_index = 0;
        int num_threads = std::thread::hardware_concurrency() - 1;
        INAE_CORE_INFO("Exporting with {} threads", num_threads);

        WebPAnimEncoder *encoder = nullptr;
        std::unique_ptr<WebPConfig> config;

        if (format == Exporter::e_WebM) {
            config = std::make_unique<WebPConfig>();
            WebPAnimEncoderOptions anim_config;
            if (!WebPAnimEncoderOptionsInit(&anim_config) || !WebPConfigInit(config.get())) {
                throw;
            }
            config->lossless = 1;
            config->thread_level = 1;
            if (!WebPValidateConfig(config.get())) {
                throw;
            }

            WebPAnimEncoderOptions enc_options;
            encoder = WebPAnimEncoderNew(m_comp->width(), m_comp->height(), &anim_config);
            if (!encoder) {
                throw;
            }
            // num_threads = 1;
        }

        int total_frames = m_comp->out_point() - m_comp->in_point();
        std::vector<ThreadContext> contexts;
        contexts.reserve(num_threads);

        for (int i = 0; i < num_threads; ++i) {
            contexts.emplace_back(m_comp, path, format, encoder, config.get());
        }

        {
            std::vector<std::thread> threads;
            threads.reserve(num_threads);
            for (int i = 0; i < num_threads; ++i) {
                threads.emplace_back(std::ref(contexts[i]));
            }

            for (int i = 0; i < num_threads; ++i) {
                try {
                    if (threads[i].joinable())
                        threads[i].join();
                } catch (...) {
                }
            }
        }

        if (format == Exporter::e_WebM) {
            WebPData webp_data;
            WebPDataInit(&webp_data);

            // add a last fake frame to signal the last duration
            bool ok = WebPAnimEncoderAdd(encoder,
                                         NULL,
                                         static_cast<int>(total_frames * 1000 / m_comp->framerate()),
                                         NULL);
            ok = ok && WebPAnimEncoderAssemble(encoder, &webp_data);
            if (!ok) {
                throw;
            }
            WebPAnimEncoderDelete(encoder);
            SkFILEWStream out(path.c_str());
            (void) out.write(webp_data.bytes, webp_data.size);
            WebPDataClear(&webp_data);
        }
    } catch (...) {
    }

    return true;
}

} // namespace inae::exporter
