#ifndef PAINTCONTEXT_H
#define PAINTCONTEXT_H

#include <optional>

class SkCanvas;
enum class SkBlendMode;
class SkPaint;

namespace alive::model {
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
    bool draw_only_self = false;
    std::optional<SkBlendMode> blend_mode;
};

class PaintContextGuard
{
public:
    PaintContextGuard(PaintContext& context) : m_context(context) {
        context.save();
    }
    ~PaintContextGuard() { m_context.restore();}
private:
    PaintContext& m_context;
};

class PaintContextSaveLayerGuard
{
public:
    PaintContextSaveLayerGuard(PaintContext& context, SkPaint& paint);;
    ~PaintContextSaveLayerGuard() { m_context.restore();}
private:
    PaintContext& m_context;
};
} // namespace alive::model

#endif // PAINTCONTEXT_H
