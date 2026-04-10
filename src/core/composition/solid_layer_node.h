#ifndef SOLIDCOMPOSITIONLAYER_H
#define SOLIDCOMPOSITIONLAYER_H

#include "layer_node.h"
#include <include/core/SkColor.h>

namespace alive::model {
class SolidLayer;
class SolidLayerNode : public LayerNode
{
public:
    SolidLayerNode(SolidLayer *solid_layer);
    void draw_layer(PaintContext &context) const override;

    int width() const;
    int height() const;
    const Color4ub &color() const;

    void update_layer(UpdateContext &context) override;

private:
    const SolidLayer *m_layer;
};

} // namespace alive::model
#endif // SOLIDCOMPOSITIONLAYER_H
