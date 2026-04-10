#include "rectangle.h"

namespace alive::model {

const ObjectDescriptor &rectangle_descriptor()
{
    static const auto rectangle_descriptor = ObjectDescriptor("RectanglePath",
                                                              ObjectDescriptor::e_RectanglePath);
    return rectangle_descriptor;
}

Rectangle::Rectangle(Object *object)
    : PathShapeItem(ShapeType::e_Rectangle, object, rectangle_descriptor())
{
}

Rectangle::Rectangle(Object *object, const Rectangle &other)
    : PathShapeItem(object, other)
    , m_size(this, other.m_size)
    , m_position(this, other.m_position)
    , m_roundness(this, other.m_roundness)
{}

Rectangle *Rectangle::clone_shape_item(Object *object) const
{
    return new Rectangle(object, *this);
}

} // namespace alive::model
