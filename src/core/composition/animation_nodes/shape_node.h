#ifndef SHAPENODE_H
#define SHAPENODE_H

#include "animation_node.h"
#include "shape_path_node.h"
#include <core/model/property/dynamic_property.h>
#include <core/path_primitives/bezier_path.h>

namespace alive::model {
class Shape;

class ShapeNode : public ShapePathNode
{
public:
    ShapeNode(const Shape *shape);
    bool update(UpdateContext &context) override;

private:
    const Shape *m_shape;
    alive::model::PropertyAnimatorUniquePtr<core::BezierPath> m_pathProp;
};

} // namespace alive::model
#endif // SHAPENODE_H
