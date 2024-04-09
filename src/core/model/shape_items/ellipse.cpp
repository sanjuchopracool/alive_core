#include "ellipse.h"

namespace inae::model {

const ObjectDescriptor &ellipse_descriptor()
{
    static const auto ellipse_descriptor = ObjectDescriptor("EllipsePath",
                                                            ObjectDescriptor::e_EllipsePath);
    return ellipse_descriptor;
}

Ellipse::Ellipse(Object *object)
    : PathShapeItem(ShapeType::Ellipse, object, ellipse_descriptor())
{
}

} // namespace inae::model
