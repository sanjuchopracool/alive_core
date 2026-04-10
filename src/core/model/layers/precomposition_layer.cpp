#include "precomposition_layer.h"
#include <core/model/composition.h>
#include <core/model/property/property_group.h>
#include <core/script/expression_engine.h>
#include <core/serializer/lottie_parser.h>

namespace alive::model {
inline const auto k_precomp_layer_descriptor = LayerDescriptor("Precomp", LayerType::e_Precomp);
PrecompositionLayer::PrecompositionLayer(Object *object, int index, const std::string &path)
    : Layer(object, k_precomp_layer_descriptor)
{
    set_layer_index(index);
    set_precomp_path(path);
}

PrecompositionLayer *PrecompositionLayer::clone_layer(Object *object, int index) const
{
    return new PrecompositionLayer(object, *this, index);
}

const Layers &PrecompositionLayer::layers() const
{
    return m_comp->layers();
}

Layers &PrecompositionLayer::layers()
{
    return m_comp->layers();
}

bool PrecompositionLayer::set_precomp_path(const std::string &path)
{
    if (m_precomp_path != path) {
        m_precomp_path = path;

        if (!path.empty()) {
            Messages messages;
            m_comp.reset(LottieParser::parse_alive_precomp(path, messages).release());
        } else {
            m_comp.reset();
        }

        notify_observers();
        if (m_comp) {
            observe(m_comp.get());
        }
        return true;
    }
    return false;
}

void PrecompositionLayer::sync_in_out_with_precomp()
{
    if (m_comp) {
        set_in_point(m_comp->in_point());
        set_out_point(m_comp->out_point());
    }
}

PropertyGroup *PrecompositionLayer::property_group()
{
    if (m_comp) {
        return m_comp->property_group();
    }

    return nullptr;
}

const PropertyGroup *PrecompositionLayer::property_group() const
{
    if (m_comp) {
        return m_comp->property_group();
    }

    return nullptr;
}

void PrecompositionLayer::apply_scripts_on_layers(script::ExpressionEngineInterface *engine)
{
    m_engine = engine;
    if (m_comp) {
        m_comp->apply_scripts_on_layers(engine);
        apply_script();
    }
}

void PrecompositionLayer::on_update(ISubject *s)
{
    if (m_applying_script)
        return;

    if (m_comp && s == m_comp->property_group()) {
        apply_script();
    }
    Layer::on_update(s);
}

Size2Di PrecompositionLayer::precomp_size() const
{
    if (m_comp) {
        return {m_comp->width(), m_comp->height()};
    }

    return {0, 0};
}

PrecompositionLayer::PrecompositionLayer(Object *object, const PrecompositionLayer &other, int index)
    : Layer(object, other)
    , m_engine(other.m_engine)
{
    set_layer_index(index);
    set_precomp_path(other.m_precomp_path);

    // update properties from other
    m_comp->property_group()->assign_values_from_other(other.property_group());
}

void PrecompositionLayer::apply_script()
{
    if (m_engine) {
        m_applying_script = true;
        auto *eng = static_cast<script::ExpressionEngine *>(m_engine);
        eng->update_comp(m_comp.get());
        eng->is_valid_expression(m_comp->apply_script());
        eng->reset_comp();
        m_applying_script = false;
    }
}

} // namespace alive::model
