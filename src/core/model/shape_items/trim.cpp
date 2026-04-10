#include "trim.h"

namespace alive::model {

const ObjectDescriptor &trim_descriptor()
{
    static const auto trim_descriptor = ObjectDescriptor("Trim", ObjectDescriptor::e_Trim);
    return trim_descriptor;
}

Trim::TrimType type_from_int(int i) {
    auto result = Trim::TrimType::e_Simultaneously;
    if (i == 2)
        result = Trim::TrimType::e_Individually;

    return result;
}

Trim::Trim(Object *object)
    : ShapeItem(ShapeType::e_Trim, object, trim_descriptor())
{
}

Trim::Trim(Object *object, const Trim &other)
    : ShapeItem(object, other)
    , m_start(this, other.m_start)
    , m_end(this, other.m_end)
    , m_offset(this, other.m_offset)
{}

Trim *Trim::clone_shape_item(Object *object) const
{
    return new Trim(object, *this);
}
} // namespace alive::model
