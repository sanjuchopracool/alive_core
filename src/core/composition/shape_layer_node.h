#ifndef SHAPE_LAYER_NODE_H
#define SHAPE_LAYER_NODE_H

#include "layer_node.h"
#include <core/composition/animation_nodes/group_node.h>

namespace inae::model {
class ShapeLayer;
class Object;

class ShapeLayerNode : public LayerNode, public GroupNode
{
public:
    ShapeLayerNode(ShapeLayer *layer);
    ~ShapeLayerNode();

    bool on_shape_added(Group *group, ShapeItem *shape, int shape_index);
    bool on_shape_removed(Group *group, ShapeItem *shape);
    bool on_shapes_moved(model::Object *group, int from, int to);

protected:
    void draw_layer(PaintContext &context) const override;
    void update_layer(UpdateContext &context) override;
};

} // namespace inae::model
#endif // SHAPE_LAYER_NODE_H
