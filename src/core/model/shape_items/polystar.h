#ifndef POLYSTAR_H
#define POLYSTAR_H

#include <model/property/final_properties.h>
#include <model/shape_items/shape_item.h>

namespace inae::core {
class Serializer;
}

namespace inae::model {
class PolyStar : public PathShapeItem
{
    ADD_POINTS
    ADD_POSITION_2D
    ADD_ROTATION
public:
    PolyStar();

    enum class Type {
        e_Star,
        e_Polygon,
    };

public:
    //    PropertyPtr<Vec1D> m_outer_radius;
    //    PropertyPtr<Vec1D> m_outer_roundness;
    //    PropertyPtr<Vec1D> m_inner_radius;
    //    PropertyPtr<Vec1D> m_inner_roundness;
    //    PropertyPtr<Vec1D> m_num_points;

    Type m_star_type = Type::e_Polygon;
};
} // namespace inae::model

#endif // POLYSTAR_H
