#ifndef ANIMATIONNODEFACTORY_H
#define ANIMATIONNODEFACTORY_H

#include <memory>

namespace alive::model {
class ShapeAnimationNode;
class PathNode;
class ShapeItem;

namespace AnimationNodeFactory {

std::unique_ptr<ShapeAnimationNode> node_for_shape_item(const ShapeItem *shape_item);
}
} // namespace alive::model
#endif // ANIMATIONNODEFACTORY_H
