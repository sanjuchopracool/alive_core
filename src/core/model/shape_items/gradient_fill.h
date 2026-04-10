#ifndef GRADIENT_FILL_H
#define GRADIENT_FILL_H

#include "shape_properties_macros.h"
#include <core/fill_helper.h>
#include <core/model/property/final_properties.h>
#include <core/model/shape_items/shape_item.h>

namespace alive::core {
class Serializer;
}

namespace alive::model {
class GradientFill : public ShapeItem
{
    ADD_OPACITY
public:
    GradientFill(Object *object = nullptr);

private:
    GradientType m_type = GradientType::e_Linear;

private:
    ADD_FRIEND_SERIALIZERS
};
} // namespace alive::model

#endif // GRADIENT_FILL_H
