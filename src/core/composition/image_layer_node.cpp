#include "image_layer_node.h"
#include <core/composition/animation_nodes/animation_node.h>
#include <core/composition/paint_context.h>
#include <include/core/SkCanvas.h>
#include <include/core/SkData.h>
#include <core/logging/logger.h>
#include <core/model/layers/image_layer.h>

namespace alive::model {
ImageLayerNode::ImageLayerNode(ImageLayer *image_layer)
    : LayerNode(image_layer)
    , m_image_layer(image_layer)
{
    load_image();
}

void ImageLayerNode::draw_layer(PaintContext &context) const
{
    if (m_image) {
        context.canvas.drawImage(m_image, 0, 0);
    }
}

void ImageLayerNode::update_layer(UpdateContext &context)
{
    LayerNode::update_base_layer_node(context);
}

void ImageLayerNode::on_update(ISubject *subject)
{
    if (subject == m_image_layer) {
        load_image();
    }

    CascadeUpdateListener::on_update(subject);
}

void ImageLayerNode::load_image()
{
    m_image.reset();
    sk_sp<SkData> data = SkData::MakeFromFileName(m_image_layer->path().c_str());
    if (data) {
        m_image = SkImages::DeferredFromEncodedData(data);
        ALIVE_CORE_TRACE("Image Size({})= ({}x{})",
                        m_image_layer->path().c_str(),
                        m_image->width(),
                        m_image->height());
    }
    if (!m_image) {
        ALIVE_CORE_ERROR("Failed to load image data from file {} !", m_image_layer->path());
    }
}
} // namespace alive::model
