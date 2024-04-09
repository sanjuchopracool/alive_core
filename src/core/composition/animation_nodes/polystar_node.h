#ifndef POLYSTARNODE_H
#define POLYSTARNODE_H

#include <utility/primitives/utility.h>
#include "animation_node.h"
#include "shape_path_node.h"
#include <model/property_system/property.h>

#include <memory>

namespace inae::core {
class PolyStar;

class PolyStarNode : public ShapePathNode
{
public:
    PolyStarNode(const PolyStar &polystar);
    bool update(FrameType t, bool force_update) override;

private:
    PropertyAnimatorPtr<QVector2D> m_center;
    PropertyAnimatorPtr<Vector1D> m_rotation;
    PropertyAnimatorPtr<Vector1D> m_outer_radius;
    PropertyAnimatorPtr<Vector1D> m_outer_roundness;
    PropertyAnimatorPtr<Vector1D> m_inner_radius;
    PropertyAnimatorPtr<Vector1D> m_inner_roundness;
    PropertyAnimatorPtr<Vector1D> m_num_points;

    QPainterPath m_star_path;
};
}

#endif // POLYSTARNODE_H
