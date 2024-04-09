#include "ellipse_node.h"

#include <core/model/shape_items/ellipse.h>
#include <include/core/SkRect.h>
#include <core/model/property/dynamic_property_animator.h>

namespace inae::model {

constexpr float k_ellipse_control_point_percentage = 0.55228f;

EllipseNode::EllipseNode(const Ellipse *ellipse)
    : ShapePathNode(ellipse)
    , m_ellipse(ellipse)
    , m_size(create_animator(ellipse->size(), this))
    , m_position(create_animator(ellipse->position(), this))
{
}

bool EllipseNode::update(UpdateContext &context)
{
    bool result = false;
    m_size->update(context);
    m_position->update(context);

    // TODO listen to trim paths also
    if (context.force_update || m_dirty || m_ellipse->is_dirty()) {
        create_path();
        m_dirty = false;
        result = true;
    }
    return result;
}

void EllipseNode::create_path()
{
    m_path.reset();

    if (m_ellipse->hidden()) {
        return;
    }

    Vec2D size = m_size->value();
    Vec2D position = m_position->value();
    Vec1D half_width = size.x / 2.f;
    Vec1D half_height = size.y / 2.f;

    Vec1D el_w = half_width * k_ellipse_control_point_percentage;
    Vec1D el_h = half_height * k_ellipse_control_point_percentage;

    if (m_ellipse->path_direction() == PathDirection::e_CounterClockwise) {
        m_path.moveTo(0, -half_height);
        m_path.cubicTo(0 - el_w, -half_height, -half_width, 0 - el_h, -half_width, 0);
        m_path.cubicTo(-half_width, 0 + el_h, 0 - el_w, half_height, 0, half_height);
        m_path.cubicTo(0 + el_w, half_height, half_width, 0 + el_h, half_width, 0);
        m_path.cubicTo(half_width, 0 - el_h, 0 + el_w, -half_height, 0, -half_height);
    } else {
        m_path.moveTo(0, -half_height);
        m_path.cubicTo(0 + el_w, -half_height, half_width, 0 - el_h, half_width, 0);
        m_path.cubicTo(half_width, 0 + el_h, 0 + el_w, half_height, 0, half_height);
        m_path.cubicTo(0 - el_w, half_height, -half_width, 0 + el_h, -half_width, 0);
        m_path.cubicTo(-half_width, 0 - el_h, 0 - el_w, -half_height, 0, -half_height);
    }
    m_path.close();

    m_path.offset(position.x, position.y, nullptr);
    apply_trim();
}
} // namespace inae::model
