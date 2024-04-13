#include "solid_layer.h"

namespace inae::model {

const LayerDescriptor &solid_layer_descriptor()
{
    static const auto solid_layer_descriptor = LayerDescriptor("SolidLayer", LayerType::e_Solid);
    return solid_layer_descriptor;
}
SolidLayer::SolidLayer(Object *object, int index)
    : Layer(object, solid_layer_descriptor())
{
    set_layer_index(index);
}

SolidLayer::~SolidLayer() {}

} // namespace inae::model
