#ifndef PAINTCONTEXT_H
#define PAINTCONTEXT_H

#include <optional>

class SkCanvas;
enum class SkBlendMode;

namespace inae::model {
struct PaintContext
{
public:
    PaintContext(SkCanvas &in, int alpha = 255);
    PaintContext(const PaintContext &other);

    void save();
    void restore();

    SkCanvas &canvas;
    int parent_alpha = 255;
    bool is_matte_context = false;
    std::optional<SkBlendMode> blend_mode;
};
} // namespace inae::model

#endif // PAINTCONTEXT_H
