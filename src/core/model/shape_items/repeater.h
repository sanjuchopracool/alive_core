#ifndef REPEATER_H
#define REPEATER_H

#include "repeater_properties.h"
#include "shape_transformation.h"
#include <core/model/property_system/property.h>

namespace alive::core {
class Serializer;
}
namespace alive::model {

class Repeater : public ShapeItem
{
    ADD_COPIES
    ADD_OFFSET

public:
    Repeater(Object *object = nullptr);
    Repeater(Object *object, const Repeater &other);
    const ShapeTransformation *transform() const { return &m_transformation; }

    Repeater *clone_shape_item(Object *object) const override;

public:
    ShapeTransformation m_transformation;
    int m_composite = 1;

private:
    ADD_FRIEND_SERIALIZERS
};
} // namespace alive::model

#endif // REPEATER_H
