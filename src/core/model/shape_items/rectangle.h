#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <core/model/property/final_properties.h>
#include <core/model/shape_items/shape_item.h>

namespace inae::core {
class Serializer;
}

namespace inae::model {
class Rectangle : public PathShapeItem
{
    ADD_SIZE_2D
    ADD_POSITION_2D
    ADD_ROUNDNESS
public:
    Rectangle(Object *object = nullptr);

private:
    friend class inae::core::Serializer;
};
} // namespace inae::model

#endif // RECTANGLE_H
