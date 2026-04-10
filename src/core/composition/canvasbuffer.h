#ifndef CANVASBUFFER_H
#define CANVASBUFFER_H

#include <Corrade/Containers/Pointer.h>
#include <core/alive_types/types.h>
#include <vector>

class SkCanvas;
namespace alive {

namespace model {
class CompositionNode;
}
class CanvasBuffer
{
public:
    enum BufferFormat { e_RGBA, e_BGRA };
    CanvasBuffer();
    ~CanvasBuffer();
    void resize(int32_t width, int32_t height);
    void draw(model::CompositionNode *container, const Matrix3D &matrix, bool draw_background);
    int32_t width() const { return m_width; }
    int32_t height() const { return m_height; }
    BufferFormat format() const { return m_format; }
    const uint8_t *data() const { return  m_pixels.data(); }
    std::size_t min_row_bytes() const { return m_min_row_bytes; }
    bool empty() const { return m_pixels.empty(); }

protected:
    Corrade::Containers::Pointer<SkCanvas> m_canvas;
    std::vector<uint8_t> m_pixels;
    BufferFormat m_format;
    int32_t m_width;
    int32_t m_height;
    std::size_t m_min_row_bytes;
};
} // namespace alive

#endif // CANVASBUFFER_H
