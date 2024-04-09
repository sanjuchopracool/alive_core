#include "trim.h"

namespace inae::model {

const ObjectDescriptor &trim_descriptor()
{
    static const auto trim_descriptor = ObjectDescriptor("Trim", ObjectDescriptor::e_Trim);
    return trim_descriptor;
}

Trim::TrimType type_from_int(int i) {
    auto result = Trim::TrimType::Simultaneously;
    if (i == 2)
        result = Trim::TrimType::Individually;

    return result;
}

Trim::Trim(Object *object)
    : ShapeItem(ShapeType::Trim, object, trim_descriptor())
{
}
} // namespace inae::model
