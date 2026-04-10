#include "shape_transformation.h"

namespace alive::model {

const ObjectDescriptor &shape_transformation_descriptor()
{
    static const auto shape_transformation_descriptor
        = ObjectDescriptor("Transformation", ObjectDescriptor::e_ShapeTransformation);
    return shape_transformation_descriptor;
}

ShapeTransformation::ShapeTransformation(Object *object)
    : ShapeItem(ShapeType::e_Transform, object, shape_transformation_descriptor())
{
}

ShapeTransformation::ShapeTransformation(Object *object, const ShapeTransformation &other)
    : ShapeItem(object, other)
    , m_anchor(this, other.m_anchor)
    , m_position(this, other.m_position)
    , m_scale(this, other.m_scale)
    , m_rotation(this, other.m_rotation)
    , m_opacity(this, other.m_opacity)
    , m_skew(this, other.m_skew)
    , m_skew_axis(this, other.m_skew_axis)
{}

ShapeTransformation *ShapeTransformation::clone_shape_item(Object *object) const
{
    return new ShapeTransformation(object, *this);
}

} // namespace alive::model
