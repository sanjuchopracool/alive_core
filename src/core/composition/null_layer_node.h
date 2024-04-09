#ifndef NULLCOMPOSITIONLAYER_H
#define NULLCOMPOSITIONLAYER_H

#include "layer_node.h"

namespace inae::model {
class NullLayerNode : public LayerNode
{
public:
    NullLayerNode(Layer *layer);

protected:
    void draw_layer(PaintContext &context) const override {}
    void update_layer(UpdateContext &context) override;
};

} // namespace inae::model

#endif // NULLCOMPOSITIONLAYER_H
