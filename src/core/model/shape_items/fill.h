#ifndef FILL_H
#define FILL_H

#include "shape_properties_macros.h"
#include <core/fill_helper.h>
#include <core/model/property/final_properties.h>
#include <core/model/shape_items/shape_item.h>

namespace alive::core {
class Serializer;
}

namespace alive::model {
class Fill : public ShapeItem
{
    ADD_COLOR
    ADD_OPACITY
public:
    Fill(Object *object = nullptr);
    Fill(Object *object, const Fill &other);

    Fill *clone_shape_item(Object *object) const override;

public:
    FillRule m_fill_rule = FillRule::e_NonZeroWinding;
    bool m_fillEnabled = true;

private:
    ADD_FRIEND_SERIALIZERS
};
} // namespace alive::model

#endif // FILL_H
