#include "rectangle_node.h"

#include <core/model/shape_items/rectangle.h>
#include <include/core/SkRect.h>
#include <core/model/property/dynamic_property_animator.h>

namespace inae::model {
RectangleNode::RectangleNode(const Rectangle *rectangle)
    : ShapePathNode(rectangle)
    , m_rectangle(rectangle)
    , m_size(create_animator(rectangle->size(), this))
    , m_position(create_animator(rectangle->position(), this))
    , m_roundness(create_animator(rectangle->roundness(), this))
{
}

bool RectangleNode::update(UpdateContext &context)
{
    bool result = false;
    m_size->update(context);
    m_position->update(context);
    m_roundness->update(context);

    // TODO listen to trim paths also
    if (context.force_update || m_dirty || m_rectangle->is_dirty()) {
        create_path();
        m_dirty = false;
        result = true;
    }
    return result;
}

void RectangleNode::create_path()
{
    m_path.reset();

    if (m_rectangle->hidden()) {
        return;
    }

    Vec2D size = m_size->value();
    Vec2D position = m_position->value();
    Vec1D half_w = size.x / 2.f;
    Vec1D half_h = size.y / 2.f;
    Vec1D r = m_roundness->value();

    r = std::min(r, std::min(half_h, half_w));

    auto radius_rect = [&](Vec2D center) -> SkRect {
        SkRect rect = SkRect::MakeLTRB(-r, -r, r, r);
        rect.offset(center.x, center.y);
        return rect;
    };

    m_path.moveTo(half_w, -(half_h - r));
    if (m_rectangle->path_direction() == PathDirection::e_CounterClockwise) {
        SkRect rect = radius_rect({(half_w - r), -(half_h - r)});
        m_path.arcTo(rect, 0, -90, false);
        m_path.lineTo(-(half_w - r), -half_h);
        rect = radius_rect(Vec2D{-(half_w - r), -(half_h - r)});
        m_path.arcTo(rect, -90, -90, false);
        m_path.lineTo(-half_w, (half_h - r));
        rect = radius_rect(Vec2D{-(half_w - r), half_h - r});
        m_path.arcTo(rect, -180, -90, false);
        m_path.lineTo(half_w - r, half_h);
        rect = radius_rect(Vec2D{half_w - r, half_h - r});
        m_path.arcTo(rect, -270, -90, false);
    } else {
        m_path.lineTo(half_w, (half_h - r));
        SkRect rect = radius_rect({half_w - r, half_h - r});
        m_path.arcTo(rect, 0, 90, false);
        m_path.lineTo(-(half_w - r), half_h);
        rect = radius_rect({-(half_w - r), half_h - r});
        m_path.arcTo(rect, 90, 90, false);
        m_path.lineTo(-half_w, -(half_h - r));
        rect = radius_rect({-(half_w - r), -(half_h - r)});
        m_path.arcTo(rect, 180, 90, false);
        m_path.lineTo(half_w - r, -half_h);
        rect = radius_rect({(half_w - r), -(half_h - r)});
        m_path.arcTo(rect, 270, 90, false);
    }
    m_path.close();

    m_path.offset(position.x, position.y, nullptr);
    apply_trim();
}
} // namespace inae::model
