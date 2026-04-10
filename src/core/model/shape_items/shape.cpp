#include "shape.h"

namespace alive::model {
const ObjectDescriptor &shape_path_descriptor()
{
    static const auto shape_path_descriptor = ObjectDescriptor("ShapePath",
                                                               ObjectDescriptor::e_ShapePath);
    return shape_path_descriptor;
}

Shape::Shape(Object *object)
    : PathShapeItem(ShapeType::e_Shape, object, shape_path_descriptor())
{
}

Shape::~Shape()
{

}

Shape::Shape(Object *object, const Shape &other)
    : PathShapeItem(object, other)
    , m_path(this, other.m_path)
{}

Shape *Shape::clone_shape_item(Object *object) const
{
    return new Shape(object, *this);
}
} // namespace alive::model
