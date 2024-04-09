#ifndef RECTANGLENODE_H
#define RECTANGLENODE_H

#include "animation_node.h"
#include "shape_path_node.h"
#include <core/model/property/dynamic_property.h>

namespace inae::model {

class Rectangle;

class RectangleNode : public ShapePathNode
{
public:
    RectangleNode(const Rectangle *rectangle);
    bool update(UpdateContext &context) override;

private:
    void create_path();

private:
    const inae::model::Rectangle *m_rectangle = nullptr;
    inae::model::PropertyAnimatorUniquePtr<Vec2D> m_size;
    inae::model::PropertyAnimatorUniquePtr<Vec2D> m_position;
    inae::model::PropertyAnimatorUniquePtr<Vec1D> m_roundness;
};

} // namespace inae::model

#endif // RECTANGLENODE_H
