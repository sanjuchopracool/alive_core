#include "solid_layer.h"

namespace alive::model {

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

SolidLayer *SolidLayer::clone_layer(Object *object, int index) const
{
    return new SolidLayer(object, *this, index);
}

SolidLayer::SolidLayer(Object *object, const SolidLayer &other, int index)
    : Layer(object, other)
    , m_color(other.m_color)
    , m_height(other.m_height)
    , m_width(other.m_width)
{
    set_layer_index(index);
}

} // namespace alive::model
