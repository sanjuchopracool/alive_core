#ifndef IMAGELAYERNODE_H
#define IMAGELAYERNODE_H

#include "layer_node.h"
#include <include/core/SkImage.h>

namespace alive::model {
class ImageLayer;

class ImageLayerNode : public LayerNode
{
public:
    ImageLayerNode(ImageLayer *image_layer);
    void draw_layer(PaintContext &context) const override;
    void update_layer(UpdateContext &context) override;

private:
    void on_update(ISubject *subject) override;
    void load_image();

private:
    const ImageLayer *m_image_layer;
    sk_sp<SkImage> m_image;
};
} // namespace alive::model

#endif // IMAGELAYERNODE_H
