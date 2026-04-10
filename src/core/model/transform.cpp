#include "transform.h"
namespace alive::model {

const ObjectDescriptor &transform_descriptor()
{
    static auto transform_descriptor = ObjectDescriptor("Transform", ObjectDescriptor::e_Transform);
    transform_descriptor.set_support_visibility(false);
    return transform_descriptor;
}
Transform::Transform(Object *object)
    : Object(object, transform_descriptor())
{}

Transform::Transform(Object *object, const Transform &other)
    : Object(object, transform_descriptor(), other.name())
    , m_anchor(this, other.m_anchor)
    , m_position(this, other.m_position)
    , m_scale(this, other.m_scale)
    , m_rotation(this, other.m_rotation)
    , m_opacity(this, other.m_opacity)
    , m_skew(this, other.m_skew)
    , m_skew_axis(this, other.m_skew_axis)
{}

Transform *Transform::clone(Object *object) const
{
    return new Transform(object, *this);
}
} // namespace alive::model
