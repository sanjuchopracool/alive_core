#ifndef ANIMATIONNODEFACTORY_H
#define ANIMATIONNODEFACTORY_H

#include <memory>

namespace inae::model {
class ShapeAnimationNode;
class PathNode;
class ShapeItem;

namespace AnimationNodeFactory {

std::unique_ptr<ShapeAnimationNode> node_for_shape_item(const ShapeItem *shape_item);
}
} // namespace inae::model
#endif // ANIMATIONNODEFACTORY_H
