#include "canvasbuffer.h"
#include <core/composition/composition_node.h>
#include <core/logging/logger.h>
#include <core/skia_conversion/skia_to_alive.h>
#include <include/core/SkCanvas.h>
#include <include/core/SkImageInfo.h>

namespace alive {

void CanvasBuffer::resize(int32_t width, int32_t height)
{
    m_width = width;
    m_height = height;
    SkImageInfo info = SkImageInfo::Make(width, height,kRGBA_8888_SkColorType, kPremul_SkAlphaType);
    m_min_row_bytes = info.minRowBytes();
    if (info.colorType() == kRGBA_8888_SkColorType) {
        m_format = e_RGBA;
    } else if (info.colorType() == kBGRA_8888_SkColorType) {
        m_format = e_BGRA;
    } else {
        ALIVE_CORE_CRITICAL("Unknown buffer format {}", static_cast<int>(info.colorType()));
    }

    m_pixels.resize(info.computeMinByteSize());
    m_canvas.reset(SkCanvas::MakeRasterDirect(info, &m_pixels[0], info.minRowBytes()).release());
}

void CanvasBuffer::draw(model::CompositionNode *container,
                        const alive::Matrix3D &matrix,
                        bool draw_background)
{
    m_canvas->save();

    if (draw_background) {
        m_canvas->clear(alive::skia::to_sk_color(container->canvas_background_color()));
    } else {
        m_canvas->clear(SK_ColorTRANSPARENT);
    }
    m_canvas->concat(skia::to_skmatrix(matrix));
    container->draw(m_canvas.get());
    m_canvas->restore();
}

CanvasBuffer::CanvasBuffer() {}

CanvasBuffer::~CanvasBuffer() {}
} // namespace alive
