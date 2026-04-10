#include "repeater.h"

namespace alive::model {

inline const auto k_repeater_descriptor = ObjectDescriptor("Repeater", ObjectDescriptor::e_Repeater);

Repeater::Repeater(Object *object)
    : ShapeItem(ShapeType::e_Repeater, object, k_repeater_descriptor)
    , m_transformation(this)
{
    m_transformation.set_position(Vec2D{100, 0});
}

Repeater::Repeater(Object *object, const Repeater &other)
    : ShapeItem(object, other)
    , m_transformation(this, other.m_transformation)
{}

Repeater *Repeater::clone_shape_item(Object *object) const
{
    return new Repeater(object, *this);
}
} // namespace alive::model
