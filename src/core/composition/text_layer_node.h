#ifndef TEXTLAYERNODE_H
#define TEXTLAYERNODE_H

#include "layer_node.h"
#include <core/model/property/dynamic_property.h>
#include <core/model/text_primitives/text_document_property.h>

namespace inae::model {

class TextLayer;

class TextLayerNode : public LayerNode
{
public:
    TextLayerNode(TextLayer *text_layer);
    void draw_layer(PaintContext &context) const override;
    void update_layer(UpdateContext &context) override;

    const core::TextDocumentProperty &text() const;

private:
    const TextLayer *m_text_layer;
    inae::model::PropertyAnimatorUniquePtr<core::TextDocumentProperty> m_text;
};
} // namespace inae::model

#endif // TEXTLAYERNODE_H
