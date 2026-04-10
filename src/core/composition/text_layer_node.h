#ifndef TEXTLAYERNODE_H
#define TEXTLAYERNODE_H

#include "layer_node.h"
#include <core/model/property/dynamic_property.h>
#include <core/model/text_primitives/text.h>

namespace alive::model {

class TextLayer;

class TextLayerNode : public LayerNode
{
public:
    TextLayerNode(TextLayer *text_layer);
    void draw_layer(PaintContext &context) const override;
    void update_layer(UpdateContext &context) override;

    const core::Text &text() const;

private:
    const TextLayer *m_text_layer;
    alive::model::PropertyAnimatorUniquePtr<core::Text> m_text;
};
} // namespace alive::model

#endif // TEXTLAYERNODE_H
