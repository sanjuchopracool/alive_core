#ifndef POLYSTAR_H
#define POLYSTAR_H

#include "shape_properties_macros.h"
#include <core/model/shape_items/shape_item.h>

namespace alive::model {
class PolyStar : public PathShapeItem
{
    ADD_POINTS
    ADD_POSITION_2D
    ADD_ROTATION
    ADD_INNER_RADIUS
    ADD_OUTER_RADIUS
    ADD_INNER_ROUNDNESS
    ADD_OUTER_ROUNDNESS
public:
    PolyStar(Object *object = nullptr);
    PolyStar(Object *object, const PolyStar &other);

    PolyStar *clone_shape_item(Object *object) const override;

    enum class Type {
        e_Star = 1,
        e_Polygon,
    };

    Type polystar_type() const { return m_polystar_type; }
    void set_polystar_type(Type type);

private:
    Type m_polystar_type = Type::e_Polygon;

private:
    ADD_FRIEND_SERIALIZERS
};
} // namespace alive::model

#endif // POLYSTAR_H
