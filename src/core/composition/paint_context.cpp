#include "paint_context.h"
#include <include/core/SkCanvas.h>
namespace inae::model {

PaintContext::PaintContext(SkCanvas &in, int alpha)
    : canvas(in)
    , parent_alpha(alpha)
{}

PaintContext::PaintContext(const PaintContext &other)
    : canvas(other.canvas)
    , parent_alpha(other.parent_alpha)
    , blend_mode(other.blend_mode)
    , is_matte_context(other.is_matte_context)
{}

void PaintContext::save()
{
    canvas.save();
}

void PaintContext::restore()
{
    canvas.restore();
}

} // namespace inae::model
