#include "layer.h"
#include "null_layer.h"
#include "precomposition_layer.h"
#include "shape_layer.h"
#include "text_layer.h"

namespace alive::model {

inline const std::vector<std::string> k_layer_type_names{"Precomp",
                                                         "Solid",
                                                         "Image",
                                                         "Null",
                                                         "Shape",
                                                         "Text"};
Layer::Layer(Object *object, const LayerDescriptor &descriptor)
    : Object(object, descriptor, {}, ItemType::e_Layer)
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

Layer *Layer::clone(Object *object) const
{
    return this->clone_layer(object, -1);
}

const TextLayer *Layer::to_text_layer() const
{
    return const_cast<Layer *>(this)->to_text_layer();
}

TextLayer *Layer::to_text_layer()
{
    if (layer_type() == LayerType::e_Text) {
        return static_cast<TextLayer *>(this);
    }

    return nullptr;
}

const ShapeLayer *Layer::to_shape_layer() const
{
    return const_cast<Layer *>(this)->to_shape_layer();
}

ShapeLayer *Layer::to_shape_layer()
{
    if (layer_type() == LayerType::e_Shape) {
        return static_cast<ShapeLayer *>(this);
    }

    return nullptr;
}

const NullLayer *Layer::to_null_layer() const
{
    return const_cast<Layer *>(this)->to_null_layer();
}

NullLayer *Layer::to_null_layer()
{
    if (layer_type() == LayerType::e_Null) {
        return static_cast<NullLayer *>(this);
    }

    return nullptr;
}

const PrecompositionLayer *Layer::to_precomp_layer() const
{
    return const_cast<Layer *>(this)->to_precomp_layer();
}

PrecompositionLayer *Layer::to_precomp_layer()
{
    if (layer_type() == LayerType::e_Precomp) {
        return static_cast<PrecompositionLayer *>(this);
    }

    return nullptr;
}

Layer::Layer(Object *object, const Layer &other)
    : Object(object, other.m_descriptor, other.name(), ItemType::e_Layer)
    , m_transform(this, other.m_transform)
    , m_matte_layer_index(other.m_matte_layer_index)
    , m_in_point(other.m_in_point)
    , m_out_point(other.m_out_point)
    , m_start_time(other.m_start_time)
    , m_parent_index(other.m_parent_index)
    , m_blend_mode(other.m_blend_mode)
    , m_time_stretch(other.m_time_stretch)
    , m_matte(other.m_matte)
    , m_hidden(other.m_hidden)
    , m_locked(other.m_locked)
{}

const std::string &layer_type_name(LayerType type)
{
    return k_layer_type_names[static_cast<int>(type)];
}

} // namespace alive::model
