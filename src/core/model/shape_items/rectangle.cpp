#include "rectangle.h"

namespace inae::model {

const ObjectDescriptor &rectangle_descriptor()
{
    static const auto rectangle_descriptor = ObjectDescriptor("RectanglePath",
                                                              ObjectDescriptor::e_RectanglePath);
    return rectangle_descriptor;
}

Rectangle::Rectangle(Object *object)
    : PathShapeItem(ShapeType::Rectangle, object, rectangle_descriptor())
{
}

} // namespace inae::model
