#ifndef ELLIPSENODE_H
#define ELLIPSENODE_H

#include "animation_node.h"
#include "shape_path_node.h"
#include <core/model/property/dynamic_property.h>

namespace inae::model {
class Ellipse;

class EllipseNode : public ShapePathNode
{
public:
    EllipseNode(const Ellipse *ellipse);
    bool update(UpdateContext &context) override;

private:
    void create_path();

private:
    const Ellipse *m_ellipse = nullptr;
    PropertyAnimatorUniquePtr<Vec2D> m_size;
    PropertyAnimatorUniquePtr<Vec2D> m_position;
};
} // namespace inae::model

#endif // ELLIPSENODE_H
