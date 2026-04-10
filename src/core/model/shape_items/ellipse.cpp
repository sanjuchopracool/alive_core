#include "ellipse.h"

namespace alive::model {

const ObjectDescriptor &ellipse_descriptor()
{
    static const auto ellipse_descriptor = ObjectDescriptor("EllipsePath",
                                                            ObjectDescriptor::e_EllipsePath);
    return ellipse_descriptor;
}

Ellipse::Ellipse(Object *object)
    : PathShapeItem(ShapeType::e_Ellipse, object, ellipse_descriptor())
{
}

Ellipse::Ellipse(Object *object, const Ellipse &other)
    : PathShapeItem(object, other)
    , m_size(this, other.m_size)
    , m_position(this, other.m_position)
{}

Ellipse *Ellipse::clone_shape_item(Object *object) const
{
    return new Ellipse(object, *this);
}
} // namespace alive::model
