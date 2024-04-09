#include "composition_layer_factory.h"

#include <core/model/layers/image_layer.h>
#include <core/model/layers/layer.h>
#include <core/model/layers/shape_layer.h>
#include <core/model/layers/solid_layer.h>
#include <core/model/layers/text_layer.h>

#include "image_layer_node.h"
#include "layer_node.h"
#include "null_layer_node.h"
#include "shape_layer_node.h"
#include "solid_layer_node.h"
#include "text_layer_node.h"
#include <core/logging/logger.h>
#include <core/model/property/layer_descriptor.h>

namespace inae::model {

std::unique_ptr<LayerNode> CompositionLayerFactory::composition_layer(Layer *layer)
{
    std::unique_ptr<LayerNode> layer_node;
    switch (layer->layer_type()) {
    case LayerType::e_Shape:
        layer_node = std::make_unique<ShapeLayerNode>(static_cast<model::ShapeLayer *>(layer));
        break;
    case LayerType::e_Null:
        layer_node = std::make_unique<NullLayerNode>(layer);
        break;
    case model::LayerType::e_Solid:
        layer_node = std::make_unique<SolidLayerNode>(static_cast<model::SolidLayer *>(layer));
        break;
    case model::LayerType::e_Text:
        layer_node = std::make_unique<TextLayerNode>(static_cast<model::TextLayer *>(layer));
        break;
    case model::LayerType::e_Image:
        layer_node = std::make_unique<ImageLayerNode>(static_cast<model::ImageLayer *>(layer));
        break;
    default:
        INAE_CORE_ERROR("{} layer node not implemented yet!", layer_type_name(layer->layer_type()));
        break;
    }

    return layer_node;
}

} // namespace inae::model
