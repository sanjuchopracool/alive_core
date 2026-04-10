#include "fill.h"

namespace alive::model {

const ObjectDescriptor &fill_descriptor()
{
    static const auto fill_descriptor = ObjectDescriptor("Fill", ObjectDescriptor::e_Fill);
    return fill_descriptor;
}

Fill::Fill(Object *object)
    : ShapeItem(ShapeType::e_Fill, object, fill_descriptor())
{
}

Fill::Fill(Object *object, const Fill &other)
    : ShapeItem(object, other)
    , m_color(this, other.m_color)
    , m_opacity(this, other.m_opacity)
    , m_fill_rule(other.m_fill_rule)
    , m_fillEnabled(other.m_fillEnabled)
{}

Fill *Fill::clone_shape_item(Object *object) const
{
    return new Fill(object, *this);
}
} // namespace alive::model
