#include "polystar.h"

namespace alive::model {

const ObjectDescriptor &polystar_descriptor()
{
    static const auto polystar_descriptor = ObjectDescriptor("PolyStar",
                                                             ObjectDescriptor::e_PolyStarPath);
    return polystar_descriptor;
}

PolyStar::PolyStar(Object *object)
    : PathShapeItem(ShapeType::e_Star, object, polystar_descriptor())
{
}

PolyStar::PolyStar(Object *object, const PolyStar &other)
    : PathShapeItem(object, other)
    , m_points(this, other.m_points)
    , m_position(this, other.m_position)
    , m_rotation(this, other.m_rotation)
    , m_inner_radius(this, other.m_inner_radius)
    , m_outer_radius(this, other.m_outer_radius)
    , m_inner_roundness(this, other.m_inner_roundness)
    , m_outer_roundness(this, other.m_outer_roundness)
{}

PolyStar *PolyStar::clone_shape_item(Object *object) const
{
    return new PolyStar(object, *this);
}

void PolyStar::set_polystar_type(Type type)
{
    if (m_polystar_type != type) {
        m_polystar_type = type;
        mark_dirty(true);
        notify_observers();
        mark_dirty(false);
    }
}
} // namespace alive::model
