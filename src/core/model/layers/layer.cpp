#include "layer.h"

namespace inae::model {

inline const std::vector<std::string> k_layer_type_names{"Precomp",
                                                         "Solid",
                                                         "Image",
                                                         "Null",
                                                         "Shape",
                                                         "Text"};
Layer::Layer(Object *object, const LayerDescriptor &descriptor)
    : Object(object, descriptor, ItemType::e_Layer)
    , m_transform(this)
{}

Layer::~Layer() {}

bool Layer::in_range(FrameTimeType t) const
{
    return (m_in_point <= t) && (t <= m_out_point);
}

LayerType Layer::layer_type() const
{
    return static_cast<const LayerDescriptor &>(m_descriptor).layer_type();
}

const std::string &layer_type_name(LayerType type)
{
    return k_layer_type_names[static_cast<int>(type)];
}

} // namespace inae::model
