#ifndef RECTANGLENODE_H
#define RECTANGLENODE_H

#include "animation_node.h"
#include "shape_path_node.h"
#include <core/model/property/dynamic_property.h>

namespace alive::model {

class Rectangle;

class RectangleNode : public ShapePathNode
{
public:
    RectangleNode(const Rectangle *rectangle);
    bool update(UpdateContext &context) override;

    Vec2D size() const;

private:
    void create_path();

private:
    const alive::model::Rectangle *m_rectangle = nullptr;
    alive::model::PropertyAnimatorUniquePtr<Vec2D> m_size;
    alive::model::PropertyAnimatorUniquePtr<Vec2D> m_position;
    alive::model::PropertyAnimatorUniquePtr<Vec1D> m_roundness;
};

} // namespace alive::model

#endif // RECTANGLENODE_H
