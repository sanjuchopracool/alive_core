#ifndef POLYSTARNODE_H
#define POLYSTARNODE_H

#include "animation_node.h"
#include "shape_path_node.h"
#include <core/model/property/dynamic_property.h>

namespace alive::model {
class PolyStar;

class PolyStarNode : public ShapePathNode
{
public:
    PolyStarNode(const PolyStar *polystar);
    bool update(UpdateContext &context) override;

private:
    void create_path();

private:
    const alive::model::PolyStar *m_polystar = nullptr;
    alive::model::PropertyAnimatorUniquePtr<Vec1D> m_num_points;
    alive::model::PropertyAnimatorUniquePtr<Vec2D> m_position;
    alive::model::PropertyAnimatorUniquePtr<Vec1D> m_rotation;
    alive::model::PropertyAnimatorUniquePtr<Vec1D> m_inner_radius;
    alive::model::PropertyAnimatorUniquePtr<Vec1D> m_outer_radius;
    alive::model::PropertyAnimatorUniquePtr<Vec1D> m_inner_roundness;
    alive::model::PropertyAnimatorUniquePtr<Vec1D> m_outer_roundness;
};
} // namespace alive::model

#endif // POLYSTARNODE_H
