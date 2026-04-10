#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <core/model/property/final_properties.h>
#include <core/model/shape_items/shape_item.h>

namespace alive::model {
class Rectangle : public PathShapeItem
{
    ADD_SIZE_2D
    ADD_POSITION_2D
    ADD_ROUNDNESS
public:
    Rectangle(Object *object = nullptr);
    Rectangle(Object *object, const Rectangle &other);

    Rectangle *clone_shape_item(Object *object) const override;

private:
    ADD_FRIEND_SERIALIZERS
};
} // namespace alive::model

#endif // RECTANGLE_H
