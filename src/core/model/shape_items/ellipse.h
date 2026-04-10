#ifndef ELLIPSE_H
#define ELLIPSE_H

#include <core/model/property/final_properties.h>
#include <core/model/shape_items/shape_item.h>

namespace alive::core {
class Serializer;
}

namespace alive::model {

class Ellipse : public PathShapeItem
{
    ADD_SIZE_2D
    ADD_POSITION_2D
public:
    Ellipse(Object *object = nullptr);
    Ellipse(Object *object, const Ellipse &other);

    Ellipse *clone_shape_item(Object *object) const override;

private:
    ADD_FRIEND_SERIALIZERS
};
} // namespace alive::model

#endif // ELLIPSE_H
