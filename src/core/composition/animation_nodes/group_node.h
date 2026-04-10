#ifndef GROUP_NODE_H
#define GROUP_NODE_H

#include "animation_node.h"
#include <core/composition/interfaces/painting_element.h>
#include <memory>
#include <vector>

namespace alive::model {
class Group;
class ShapeItem;

class GroupNode : public ShapeAnimationNode, public PaintingElement
{
public:
    GroupNode(const Group *group);
    bool update(UpdateContext &context) override;

    void set_content(const std::vector<ShapeAnimationNode *> &items_before,
                     const std::vector<ShapeAnimationNode *> &items_after) override;

    bool add_shape(Group *group, ShapeItem *shape, int shape_index);
    bool remove_shape(Group *group, ShapeItem *shape);

    bool add_shape(const ShapeItem *shape, int shape_index);
    bool remove_shape(ShapeItem *shape);
    bool move_shapes(int from, int to);

protected:
    void draw(PaintContext &context) const override;
    void reset_content();

    std::vector<std::unique_ptr<ShapeAnimationNode>> m_nodes;
    std::vector<PaintingElement *> m_paint_items;
};

} // namespace alive::model
#endif // GROUP_NODE_H
