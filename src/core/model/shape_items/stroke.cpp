#include "stroke.h"

namespace alive::model {

const ObjectDescriptor &stroke_descriptor()
{
    static const auto stroke_descriptor = ObjectDescriptor("Stroke", ObjectDescriptor::e_Stroke);
    return stroke_descriptor;
}

Stroke::Stroke(Object *object)
    : ShapeItem(ShapeType::e_Stroke, object, stroke_descriptor())
{
}

Stroke::Stroke(Object *object, const Stroke &other)
    : ShapeItem(object, other)
    , m_width(this, other.m_width)
    , m_color(this, other.m_color)
    , m_opacity(this, other.m_opacity)
    , m_fillEnabled(other.m_fillEnabled)
{}

Stroke *Stroke::clone_shape_item(Object *object) const
{
    return new Stroke(object, *this);
}
} // namespace alive::model
