#ifndef SHAPETRANSFORMATION_H
#define SHAPETRANSFORMATION_H

#include "shape_properties_macros.h"
#include <core/model/property/final_properties.h>
#include <core/model/shape_items/shape_item.h>

namespace alive::core {
class Serializer;
}

namespace alive::model {

class ShapeTransformation : public ShapeItem
{
    ADD_ANCHOR_2D
    ADD_POSITION_2D
    ADD_SCALE_2D
    ADD_ROTATION
    ADD_OPACITY
    ADD_SKEW
    ADD_SKEW_AXIS
public:
    ShapeTransformation(Object *object = nullptr);
    ShapeTransformation(Object *object, const ShapeTransformation &other);

    ShapeTransformation *clone_shape_item(Object *object) const override;

private:
    ADD_FRIEND_SERIALIZERS
};

} // namespace alive::model

#endif // SHAPETRANSFORMATION_H
