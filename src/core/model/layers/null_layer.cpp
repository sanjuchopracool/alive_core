#include "null_layer.h"

namespace inae::model {
inline const auto k_null_layer_descriptor = LayerDescriptor("Null", LayerType::e_Null);
NullLayer::NullLayer(Object *object)
    : Layer(object, k_null_layer_descriptor)
{}

} // namespace inae::model
