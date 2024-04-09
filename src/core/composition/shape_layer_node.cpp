#include "shape_layer_node.h"
#include <core/model/layers/shape_layer.h>

namespace inae::model {
ShapeLayerNode::ShapeLayerNode(ShapeLayer *layer)
    : LayerNode(layer)
    , GroupNode(layer->group())
{
    reset_content();
}

ShapeLayerNode::~ShapeLayerNode() {}

void ShapeLayerNode::draw_layer(PaintContext &context) const
{
    GroupNode::draw(context);
}

void ShapeLayerNode::update_layer(UpdateContext &context)
{
    update_base_layer_node(context);
    GroupNode::update(context);
}

bool ShapeLayerNode::on_shape_added(Group *group, ShapeItem *shape, int index)
{
    if (add_shape(group, shape, index)) {
        reset_content();
        return true;
    }
    return false;
}

bool ShapeLayerNode::on_shape_removed(Group *group, ShapeItem *shape)
{
    if (remove_shape(group, shape)) {
        reset_content();
        return true;
    }
    return false;
}

bool ShapeLayerNode::on_shapes_moved(Object *group, int from, int to)
{
    bool result = false;
    if (group == shape_item()) {
        result = move_shapes(from, to);
        reset_content();
    }
    return result;
}

//void ShapeLayerNode::update_layer(FrameType t, bool force_update)
//{
//    BaseCompositionLayer::update_layer(t, force_update);
//    for (auto it = m_nodes.rbegin(); it != m_nodes.rend(); ++it) {
//        (*it)->update(t, force_update);
//    }
//}

} // namespace inae::model
