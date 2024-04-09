#include "stroke_node.h"

#include <core/model/shape_items/stroke.h>
#include "path_node.h"
#include <core/color_type.h>
#include <core/composition/paint_context.h>
#include <include/core/SkCanvas.h>
#include <include/core/SkPaint.h>
#include <core/model/property/dynamic_property_animator.h>

//#include "trim_node.h"

namespace inae::model {

//class PathGroup
//{
//public:
//    PathGroup(TrimNode *trim)
//        : m_trim(trim)
//    {}

//private:
//    TrimNode *m_trim;
//};

StrokeNode::StrokeNode(const Stroke *stroke)
    : ShapeAnimationNode(stroke)
    , m_stroke(stroke)
    , m_width(create_animator(stroke->width(), this))
    , m_color(create_animator(stroke->color(), this))
    , m_opacity(create_animator(stroke->opacity(), this))
{
    //    m_pen = stroke.m_pen;
    //TODO animate dash pattern also
    //    if (stroke->m_dashPattern.m_keyframes.size())
    //        m_dashPattern = stroke->m_dashPattern.m_keyframes[0].m_value;
    //    m_is_static = m_color->is_static() and
    //            m_opacity->is_static() and
    //            m_width->is_static();
}

bool StrokeNode::update(UpdateContext &context)
{
    m_dirty = m_stroke->is_dirty();
    m_color->update(context);
    m_opacity->update(context);
    m_width->update(context);
    return m_dirty;
}

void StrokeNode::set_content(const std::vector<ShapeAnimationNode *> &,
                             const std::vector<ShapeAnimationNode *> &items_after)
{
    m_paths.clear();
    for (const AnimationNode *node : items_after) {
        auto *path_node = dynamic_cast<const PathNode *>(node);
        if (path_node)
            m_paths.emplace_back(path_node);
    }
}

void StrokeNode::draw(PaintContext &context) const
{
    if (m_stroke->hidden())
        return;

    SkColor color = skcolor_from_vec3d(m_color->value());
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
    paint.setStyle(SkPaint::kStroke_Style);
    paint.setStrokeWidth(m_width->value());

    if (context.blend_mode) {
        paint.setBlendMode(context.blend_mode.value());
    }

    if (!path.isEmpty()) {
        context.canvas.drawPath(path, paint);
    }
}
} // namespace inae::model
