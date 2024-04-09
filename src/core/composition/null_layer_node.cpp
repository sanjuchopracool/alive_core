#include "null_layer_node.h"
#include <core/composition/animation_nodes/animation_node.h>
namespace inae::model {

NullLayerNode::NullLayerNode(Layer *layer)
    : LayerNode(layer)
{
}

void NullLayerNode::update_layer(UpdateContext &context)
{
    LayerNode::update_base_layer_node(context);
}
} // namespace inae::model
