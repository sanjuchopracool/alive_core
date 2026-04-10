#include "null_layer.h"

namespace alive::model {
inline const auto k_null_layer_descriptor = LayerDescriptor("Null", LayerType::e_Null);
NullLayer::NullLayer(Object *object, int index)
    : Layer(object, k_null_layer_descriptor)
{
    set_layer_index(index);
}

NullLayer *NullLayer::clone_layer(Object *object, int index) const
{
    return new NullLayer(object, *this, index);
}

NullLayer::NullLayer(Object *object, const NullLayer &other, int index)
    : Layer(object, other)
{
    set_layer_index(index);
}

} // namespace alive::model
