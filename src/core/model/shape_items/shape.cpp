#include "shape.h"

namespace inae::model {
const ObjectDescriptor &shape_path_descriptor()
{
    static const auto shape_path_descriptor = ObjectDescriptor("ShapePath",
                                                               ObjectDescriptor::e_ShapePath);
    return shape_path_descriptor;
}

Shape::Shape(Object *object)
    : PathShapeItem(ShapeType::Shape, object, shape_path_descriptor())
{
}

Shape::~Shape()
{

}
} // namespace inae::model
