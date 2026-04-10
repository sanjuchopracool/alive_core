#include "fill_node.h"

#include "path_node.h"
#include <core/alive_types/color_type.h>
#include <core/composition/paint_context.h>
#include <core/model/property/dynamic_property_animator.h>
#include <core/model/shape_items/fill.h>
#include <core/skia_conversion/skia_to_alive.h>
#include <include/core/SkCanvas.h>
#include <include/core/SkPaint.h>

namespace alive::model {

FillNode::FillNode(const Fill *fill)
    : ShapeAnimationNode(fill)
    , m_fill(fill)
    , m_color(create_animator(fill->color(), this))
    , m_opacity(create_animator(fill->opacity(), this))
{
    // TODO FILL group and composite
}

bool FillNode::update(UpdateContext &context)
{
    m_dirty = m_fill->is_dirty();
    m_color->update(context);
    m_opacity->update(context);
    return m_dirty;
}

void FillNode::set_content(const std::vector<ShapeAnimationNode *> &,
                           const std::vector<ShapeAnimationNode *> &items_after)
{
    m_paths.clear();
    for (const AnimationNode *node : items_after) {
        auto *path_node = dynamic_cast<const PathNode *>(node);
        if (path_node)
            m_paths.emplace_back(path_node);
    }
}

void FillNode::draw(PaintContext &context) const
{
    if (m_fill->hidden())
        return;

    SkColor color = skia::skcolor_from_vec3d(m_color->value());
    int opacity(m_opacity->value());

    if (SkColorGetA(color) == 0 || opacity == 0)
        return;

    //TODO layers above are filled multiple times
    SkPath path;
    for (auto *item : m_paths) {
        path.addPath(item->path());
    }

    SkPaint paint;
    paint.setAntiAlias(true);
    int alpha = (context.parent_alpha * SkColorGetA(color) * opacity) / (255 * 100);
    color = SkColorSetA(color, alpha);
    paint.setColor(color);
    if (context.blend_mode) {
        paint.setBlendMode(context.blend_mode.value());
    }
    if (!path.isEmpty()) {
        context.canvas.drawPath(path, paint);
    }
}
} // namespace alive::model
