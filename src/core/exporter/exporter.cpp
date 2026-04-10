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
#include <core/logging/logger.h>
#include <format>
#include <fstream>
#include <iostream>
#include <map>
#include <mutex>
#include <third_party/externals/libwebp/src/webp/encode.h>
#include <third_party/externals/libwebp/src/webp/mux.h>
#include <queue>

namespace alive::exporter {

struct Frame
{
    std::vector<uint8_t> rgba_data;
    int width;
    int height;
    int timestamp_ms;
    int frame_index;
};

struct ProcessedFrame
{
    WebPPicture picture;
    int timestamp_ms;
    int frame_index;
    bool valid;

    ProcessedFrame()
        : valid(false)
    {
        WebPPictureInit(&picture);
    }

    ~ProcessedFrame()
    {
        if (valid) {
            WebPPictureFree(&picture);
        }
    }

    // Move constructor
    ProcessedFrame(ProcessedFrame &&other) noexcept
    {
        picture = other.picture;
        timestamp_ms = other.timestamp_ms;
        frame_index = other.frame_index;
        valid = other.valid;
        other.valid = false; // Prevent double free
        WebPPictureInit(&other.picture);
    }

    // Delete copy operations
    ProcessedFrame(const ProcessedFrame &) = delete;
    ProcessedFrame &operator=(const ProcessedFrame &) = delete;
    ProcessedFrame &operator=(ProcessedFrame &&) = delete;
};

class MultiThreadWebMEncoder
{
private:
    // Input queue for raw frames
    std::mutex input_queue_mutex;
    std::condition_variable input_cv;
    std::queue<std::shared_ptr<Frame>> input_queue;

    // Output map for processed frames (ordered by frame index)
    std::mutex output_mutex;
    std::map<int, std::unique_ptr<ProcessedFrame>> processed_frames;

    std::atomic<bool> input_done{false};
    std::atomic<bool> processing_done{false};
    std::atomic<int> frames_added{0};
    std::atomic<int> frames_processed{0};
    std::atomic<int> next_frame_to_assemble{0};

    WebPAnimEncoder *anim_encoder;
    WebPAnimEncoderOptions anim_config;
    WebPConfig webp_config;
    int canvas_width, canvas_height;
    int num_worker_threads;
    int m_s_per_frame = 0;

public:
    MultiThreadWebMEncoder(int width, int height, int fps = 30, int worker_threads = 4)
        : canvas_width(width)
        , canvas_height(height)
        , num_worker_threads(worker_threads)
    {
        m_s_per_frame = 1000 / fps;

        // Initialize WebP config
        WebPConfigInit(&webp_config);
        webp_config.lossless = 1;
        webp_config.thread_level = 1; // Enable threading within WebP

        if (!WebPValidateConfig(&webp_config)) {
            throw std::runtime_error("Invalid config!");
        }
        // Initialize animation encoder options
        if (!WebPAnimEncoderOptionsInit(&anim_config)) {
            throw std::runtime_error("Invalid anim config!");
        }
        // anim_config.anim_params.loop_count = 0;
        // anim_config.minimize_size = 1;
        // anim_config.kmax = fps;
        // anim_config.allow_mixed = 1; // Allow mixed lossy/lossless

        // Create animation encoder
        anim_encoder = WebPAnimEncoderNew(width, height, &anim_config);
        if (!anim_encoder) {
            throw std::runtime_error("Failed to create WebP animation encoder");
        }
    }

    ~MultiThreadWebMEncoder()
    {
        if (anim_encoder) {
            WebPAnimEncoderDelete(anim_encoder);
        }
    }

    void addFrame(const uint8_t *rgba_data, int width, int height, int timestamp_ms, int index)
    {
        auto frame = std::make_shared<Frame>();
        frame->rgba_data.assign(rgba_data, rgba_data + (width * height * 4));
        frame->width = width;
        frame->height = height;
        frame->timestamp_ms = timestamp_ms;
        frame->frame_index = index;
        frames_added.fetch_add(1);

        {
            std::lock_guard<std::mutex> lock(input_queue_mutex);
            input_queue.push(frame);
        }
        input_cv.notify_one();
    }

    // Worker thread that processes frames into WebPPictures
    void processFramesWorker(int worker_id)
    {
        std::cout << "Worker " << worker_id << " started\n";

        while (!input_done.load() || !input_queue.empty()) {
            std::unique_lock<std::mutex> lock(input_queue_mutex);
            input_cv.wait(lock, [this] { return !input_queue.empty() || input_done.load(); });

            if (input_queue.empty())
                continue;

            auto frame = input_queue.front();
            input_queue.pop();
            lock.unlock();

            // Process frame
            auto processed = std::make_unique<ProcessedFrame>();
            if (processFrame(frame, *processed)) {
                // Store processed frame
                {
                    std::lock_guard<std::mutex> output_lock(output_mutex);
                    processed_frames[frame->frame_index] = std::move(processed);
                }

                frames_processed.fetch_add(1);
                std::cout << "Worker " << worker_id << " processed frame " << frame->frame_index
                          << "\n";
            } else {
                std::cerr << "Worker " << worker_id << " failed to process frame "
                          << frame->frame_index << "\n";
            }
        }

        std::cout << "Worker " << worker_id << " finished\n";
    }

    // Process a single frame into WebPPicture format
    bool processFrame(const std::shared_ptr<Frame> &frame, ProcessedFrame &processed)
    {
        WebPPicture &pic = processed.picture;

        // Initialize picture
        if (!WebPPictureInit(&pic)) {
            return false;
        }

        pic.width = frame->width;
        pic.height = frame->height;
        pic.use_argb = 1;

        // Allocate picture memory
        if (!WebPPictureAlloc(&pic)) {
            std::cerr << "Failed to allocate WebP picture for frame " << frame->frame_index << "\n";
            return false;
        }

        // Import RGB data directly
        if (!WebPPictureImportRGBA(&pic, frame->rgba_data.data(), frame->width * 4)) {
            std::cerr << "Failed to import RGB data for frame " << frame->frame_index << "\n";
            WebPPictureFree(&pic);
            return false;
        }

        // Set frame metadata
        processed.timestamp_ms = frame->timestamp_ms;
        processed.frame_index = frame->frame_index;
        processed.valid = true;

        return true;
    }

    // Assembly thread that adds frames to animation in correct order
    void assembleAnimation()
    {
        std::cout << "Animation assembler started\n";

        while (next_frame_to_assemble.load() < frames_added.load() || !processing_done.load()) {
            int target_frame = next_frame_to_assemble.load();

            std::unique_lock<std::mutex> lock(output_mutex);
            auto it = processed_frames.find(target_frame);

            if (it != processed_frames.end()) {
                auto &processed_frame = it->second;

                // Add frame directly to animation encoder
                // Make a copy of the config for this frame
                WebPConfig frame_config = webp_config;

                int status = WebPAnimEncoderAdd(anim_encoder,
                                                &processed_frame->picture,
                                                processed_frame->timestamp_ms,
                                                &frame_config);
                if (status) {
                    std::cout << "Assembled frame " << target_frame << " at "
                              << processed_frame->timestamp_ms << "ms\n";
                } else {
                    std::cerr << "Failed to add frame " << target_frame << " to animation\n";
                }

                // Remove processed frame
                processed_frames.erase(it);
                next_frame_to_assemble.fetch_add(1);
                lock.unlock();
            } else {
                lock.unlock();
                // Frame not ready, wait briefly
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        }

        std::cout << "Animation assembler finished\n";
    }

    void startProcessing()
    {
        // Start worker threads
        std::vector<std::thread> workers;
        for (int i = 0; i < num_worker_threads; ++i) {
            workers.emplace_back(&MultiThreadWebMEncoder::processFramesWorker, this, i);
        }

        // Start assembly thread
        std::thread assembler(&MultiThreadWebMEncoder::assembleAnimation, this);

        // Wait for workers to finish
        for (auto &worker : workers) {
            worker.join();
        }

        processing_done = true;

        // Wait for assembler to finish
        assembler.join();
    }

    bool saveToFile(const std::string &filename)
    {
        // Finalize animation
        if (!WebPAnimEncoderAdd(anim_encoder,
                                nullptr,
                                next_frame_to_assemble.load() * m_s_per_frame,
                                nullptr)) {
            std::cerr << "Failed to finalize animation\n";
            return false;
        }

        // Get animation data
        WebPData webp_data;
        WebPDataInit(&webp_data);

        if (!WebPAnimEncoderAssemble(anim_encoder, &webp_data)) {
            std::cerr << "Failed to assemble WebP animation\n";
            return false;
        }

        // Write to file
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Failed to open output file: " << filename << "\n";
            WebPDataClear(&webp_data);
            return false;
        }

        file.write(reinterpret_cast<const char *>(webp_data.bytes), webp_data.size);
        file.close();

        std::cout << "WebP animation saved to " << filename << " (" << webp_data.size
                  << " bytes)\n";
        std::cout << "Total frames: " << next_frame_to_assemble.load() << "\n";

        WebPDataClear(&webp_data);
        return true;
    }

    void finishInput()
    {
        input_done = true;
        input_cv.notify_all();
    }

    void setQuality(float quality) { webp_config.quality = quality; }

    void setMethod(int method)
    {
        webp_config.method = method; // 0-6, higher = better compression
    }

    int getFramesAdded() const { return frames_added.load(); }
    int getFramesProcessed() const { return frames_processed.load(); }
    int getFramesAssembled() const { return next_frame_to_assemble.load(); }
};

// // Simpler single-threaded version for comparison
// class SingleThreadWebMEncoder
// {
// private:
//     WebPAnimEncoder *anim_encoder;
//     WebPAnimEncoderOptions anim_config;
//     WebPConfig webp_config;
//     int frame_count;

// public:
//     SingleThreadWebMEncoder(int width, int height, int fps = 30)
//         : frame_count(0)
//     {
//         WebPConfigInit(&webp_config);
//         webp_config.quality = 85;
//         webp_config.method = 6;
//         webp_config.thread_level = 1;

//         WebPAnimEncoderOptionsInit(&anim_config);
//         anim_config.anim_params.loop_count = 0;
//         anim_config.minimize_size = 1;
//         anim_config.kmax = fps;

//         anim_encoder = WebPAnimEncoderNew(width, height, &anim_config);
//         if (!anim_encoder) {
//             throw std::runtime_error("Failed to create WebP animation encoder");
//         }
//     }

//     ~SingleThreadWebMEncoder()
//     {
//         if (anim_encoder) {
//             WebPAnimEncoderDelete(anim_encoder);
//         }
//     }

//     bool addFrame(const uint8_t *rgba_data, int width, int height, int timestamp_ms)
//     {
//         WebPPicture pic;
//         WebPPictureInit(&pic);
//         pic.width = width;
//         pic.height = height;
//         pic.use_argb = 0;

//         if (!WebPPictureAlloc(&pic)) {
//             return false;
//         }

//         if (!WebPPictureImportRGBA(&pic, rgba_data, width * 4)) {
//             WebPPictureFree(&pic);
//             return false;
//         }

//         bool success = WebPAnimEncoderAdd(anim_encoder, &pic, timestamp_ms, &webp_config);
//         WebPPictureFree(&pic);

//         if (success) {
//             frame_count++;
//             std::cout << "Added frame " << frame_count << " at " << timestamp_ms << "ms\n";
//         }

//         return success;
//     }

//     bool saveToFile(const std::string &filename)
//     {
//         WebPAnimEncoderAdd(anim_encoder, nullptr, frame_count * 33, nullptr);

//         WebPData webp_data;
//         WebPDataInit(&webp_data);

//         if (!WebPAnimEncoderAssemble(anim_encoder, &webp_data)) {
//             return false;
//         }

//         std::ofstream file(filename, std::ios::binary);
//         if (!file.is_open()) {
//             WebPDataClear(&webp_data);
//             return false;
//         }

//         file.write(reinterpret_cast<const char *>(webp_data.bytes), webp_data.size);
//         file.close();

//         std::cout << "Single-threaded WebP saved: " << webp_data.size << " bytes, " << frame_count
//                   << " frames\n";

//         WebPDataClear(&webp_data);
//         return true;
//     }
// };

class IEncoderInterface
{
public:
    virtual void init() {}
    virtual void write_frame(SkCanvas *canvas = nullptr) = 0;
    virtual void flush() {}
};

std::mutex k_encode_mutex;

Exporter::Exporter(const Composition *comp)
    : m_comp(comp)
{}

struct ThreadContext
{
    ThreadContext(const Composition *comp,
                  int start_frame,
                  int end_frame,
                  const std::string &path,
                  Exporter::Format format = Exporter::e_PNGSequence,
                  float scale = 1.0f,
                  MultiThreadWebMEncoder *encoder = nullptr)
        : m_comp(comp)
        , m_start_frame(start_frame)
        , m_end_frame(end_frame)
        , m_container(std::make_unique<CompositionNode>(m_comp))
        , m_path(path)
        , m_format(format)
        , m_scale(scale)
        , m_encoder(encoder)
    {
        if (m_format == Exporter::e_WebM && (!encoder || !WebPPictureInit(&m_pic))) {
            ALIVE_CORE_CRITICAL("web format is not supported!");
            throw "ERROR!";
        }
    }

    void operator()()
    {
        const int m_s_per_frame = 1000 / m_comp->framerate();
        SkImageInfo info = SkImageInfo::Make(m_comp->width() * m_scale,
                                             m_comp->height() * m_scale,
                                             kRGBA_8888_SkColorType,
                                             kPremul_SkAlphaType);
        sk_sp<SkSurface> raster_surface = SkSurfaces::Raster(info);
        SkCanvas *raster_canvas = raster_surface->getCanvas();
        raster_canvas->scale(m_scale, m_scale);
        if (m_start_frame > 0) {
            m_container->update(m_start_frame - 1, true);
        }
        while (m_start_frame < m_end_frame) {
            m_container->update(m_start_frame, true);
            raster_canvas->clear(SK_ColorTRANSPARENT);
            m_container->draw(raster_canvas);
            // export as png
            if (m_format == Exporter::e_PNGSequence) {
                sk_sp<SkImage> img(raster_surface->makeImageSnapshot());
                if (!img) {
                    ALIVE_CORE_CRITICAL("could not take snapshot from surface!");
                    return;
                }

                sk_sp<SkData> png = SkPngEncoder::Encode(nullptr,
                                                         img.get(),
                                                         SkPngEncoder::Options());
                if (!png) {
                    ALIVE_CORE_CRITICAL("could not create png from snapshot!");
                    return;
                }

                std::string path = m_path
                                   + std::format("/{}{:04}.png", m_comp->name(), m_start_frame);
                SkFILEWStream out(path.c_str());
                (void) out.write(png->data(), png->size());
            } else if (m_format == Exporter::e_WebM) {
                SkImageInfo info;
                size_t row_bytes;
                void *base_ptr = raster_canvas->accessTopLayerPixels(&info, &row_bytes, nullptr);
                if (!base_ptr) {
                    throw "Invalid Pointer";
                }

                m_encoder->addFrame(reinterpret_cast<uint8_t *>(base_ptr),
                                    info.width(),
                                    info.height(),
                                    m_start_frame * m_s_per_frame,
                                    m_start_frame);
                std::this_thread::sleep_for(std::chrono::milliseconds(2));

                // m_pic.width = info.width();
                // m_pic.height = info.height();
                // int status = 0;
                // if (info.colorType() == SkColorType::kRGBA_8888_SkColorType) {
                //     status = WebPPictureImportRGBA(&m_pic,
                //                                    static_cast<uint8_t *>(base_ptr),
                //                                    info.width() * 4);
                // } else {
                //     throw "Unsupported format!";
                // }
                // if (!status) {
                //     throw "Invalid frame";
                // }

                // {
                //     std::lock_guard lk(k_encode_mutex);
                //     bool ok = WebPAnimEncoderAdd(m_encoder,
                //                                  &m_pic,
                //                                  static_cast<int>(m_start_frame * m_s_per_frame),
                //                                  m_config);
                //     if (!ok) {
                //         throw;
                //     }
                // }
                // WebPPictureFree(&m_pic);
            }

            ++m_start_frame;
        }
    }

private:
    const Composition *m_comp = nullptr;
    int m_start_frame;
    int m_end_frame;
    std::unique_ptr<CompositionNode> m_container;
    std::string m_path;
    Exporter::Format m_format;
    WebPPicture m_pic;
    MultiThreadWebMEncoder *m_encoder = nullptr;
    float m_scale;
};

bool Exporter::export_as(const std::string &path, Format format, float scale)
{
    try {
        auto start_time = std::chrono::high_resolution_clock::now();
        int num_threads = std::thread::hardware_concurrency();
        ALIVE_CORE_INFO("Exporting with {} threads", num_threads);

        std::unique_ptr<MultiThreadWebMEncoder> encoder;
        std::unique_ptr<std::thread> processing_thread;

        if (format == Exporter::e_WebM) {
            encoder
                = std::make_unique<MultiThreadWebMEncoder>(static_cast<int>(m_comp->width() * scale),
                                                           static_cast<int>(m_comp->height()
                                                                            * scale),
                                                           static_cast<int>(m_comp->framerate()));

            processing_thread
                = std::make_unique<std::thread>(&MultiThreadWebMEncoder::startProcessing,
                                                encoder.get());
        }

        int total_frames = m_comp->out_point() - m_comp->in_point() + 1;
        std::vector<ThreadContext> contexts;
        contexts.reserve(num_threads);
        int frames_per_thread = total_frames / num_threads;
        int extra_frames = total_frames % num_threads;

        int frame = 0;
        for (int i = 0; i < num_threads; ++i) {
            int start = frame;
            int end = frame + frames_per_thread + ((i < extra_frames) ? 1 : 0);
            contexts.emplace_back(m_comp, start, end, path, format, scale, encoder.get());
            frame = end;
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
            encoder->finishInput();
            processing_thread->join();

            auto end_time = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time
                                                                                  - start_time);

            std::cout << "\nMulti-threaded processing completed in " << duration.count() << "ms\n";
            std::cout << "Stats: Added=" << encoder->getFramesAdded()
                      << ", Processed=" << encoder->getFramesProcessed()
                      << ", Assembled=" << encoder->getFramesAssembled() << "\n";

            if (encoder->saveToFile(path)) {
                ALIVE_CORE_INFO("Successfully saved to {}", path);
            } else {
                ALIVE_CORE_ERROR("Failed to save {}", path);
            }
        }
    } catch (...) {
    }

    return true;
}

} // namespace alive::exporter
