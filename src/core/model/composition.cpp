#include "composition.h"
#include <Corrade/Containers/Pointer.h>
#include <algorithm>
#include <core/model/layers/precomposition_layer.h>
#include <core/model/property/property_group.h>
#include <core/model/property/static_property_factory.h>

using namespace std;
namespace alive::model {

const ObjectDescriptor &composition_descriptor()
{
    static const auto composition_descriptor = ObjectDescriptor("Composition",
                                                                ObjectDescriptor::e_Composition);
    return composition_descriptor;
}

Precomposition::Precomposition(Object *object,
                               std::string name,
                               std::string version,
                               int width,
                               int height,
                               KeyFrameTime in_point,
                               KeyFrameTime out_point,
                               FrameTimeType framerate,
                               const ObjectDescriptor &descriptor)
    : Object(object, precomposition_descriptor(), name)
    , m_version(std::move(version))
    , m_width(width)
    , m_height(height)
    , m_in_point(in_point)
    , m_out_point(out_point)
    , m_framerate(framerate)
    , m_prop_group(new PropertyGroup(nullptr))
    , m_tag_manager(this)
{
    m_prop_group->set_name("Composition Properties");
    observe(m_prop_group.get());
}

Precomposition::~Precomposition() {}

void Precomposition::set_uuid(const std::string &uuid)
{
    if (m_uuid != uuid) {
        m_uuid = uuid;
        notify_observers();
    }
}

int Precomposition::index_of_layer(const Layer *layer) const
{
    int index = 0;
    for (const LayerPtr &ptr : m_layers) {
        if (ptr.get() == layer) {
            return index;
        }
        ++index;
    }

    return -1;
}

void Precomposition::add_layer_at_index(Layer *layer, int index)
{
    assert(index <= m_layers.size());
    m_layers.emplace(m_layers.begin() + index, layer);
    auto *precomp = layer->to_precomp_layer();
    if (precomp) {
        precomp->apply_scripts_on_layers(m_engine);
    }
    insert_item_at_index(layer, index);
    notify_observers();
}

Layer *Precomposition::remove_layer_at_index(int index)
{
    assert(index < m_layers.size());
    Layer *layer = m_layers[index].release();
    m_layers.erase(m_layers.begin() + index);
    remove_item(layer);
    notify_observers();
    return layer;
}

void Precomposition::remove_matte_for_layers(int matte_index)
{
    for (auto &layer : m_layers) {
        if (layer->matte_layer_index() == matte_index) {
            layer->set_matte_layer_index(-1);
        }
    }
}

bool Precomposition::move_layer(int from, int to)
{
    bool result = false;
    if ((from != to) && (from >= 0 && from < m_layers.size()) && (to >= 0 && to < m_layers.size())) {
        Layer *layer = m_layers[from].release();
        m_layers.erase(m_layers.begin() + from);
        m_layers.emplace(m_layers.begin() + to, layer);
        result = move_item(from, to);
        notify_observers();
    }

    return result;
}

int Precomposition::layers_max_index() const
{
    int index = 0;
    for (const auto &layer : m_layers) {
        index = std::max(layer->layer_index(), index);
    }
    return index + 1;
}

Precomposition *Precomposition::clone(Object *object) const
{
    return new Precomposition(object, *this);
}

Precomposition::Precomposition(Object *object, const Precomposition &other)
    : Object(object, other.m_descriptor, other.name())
    , m_version(other.m_version)
    , m_uuid(other.m_uuid)
    , m_width(other.m_width)
    , m_height(other.m_height)
    , m_in_point(other.m_in_point)
    , m_out_point(other.m_out_point)
    , m_framerate(other.m_framerate)
    , m_canvas_bg_color(other.m_canvas_bg_color)
    , m_show_canvas_bg_color(other.m_show_canvas_bg_color)
    , m_show_matte_details(other.m_show_matte_details)
    , m_prop_group(other.m_prop_group->clone(nullptr))
    , m_apply_property_on_change(other.m_apply_property_on_change)
    , m_apply_script(other.m_apply_script)
    , m_tag_manager(this)
{
    for (const auto &layer : other.m_layers) {
        m_layers.emplace_back(layer->clone_layer(this, layer->layer_index()));
    }
    observe(m_prop_group.get());
}

void Precomposition::set_width(int width)
{
    if (m_width != width) {
        m_width = width;
        notify_observers();
    }
}

void Precomposition::set_height(int height)
{
    if (m_height != height) {
        m_height = height;
        notify_observers();
    }
}

void Precomposition::set_in_point(KeyFrameTime in_point)
{
    if (m_in_point != in_point) {
        m_in_point = in_point;
        notify_observers();
    }
}

void Precomposition::set_out_point(KeyFrameTime out_point)
{
    if (m_out_point != out_point) {
        m_out_point = out_point;
        notify_observers();
    }
}

void Precomposition::set_framerate(FrameTimeType framerate)
{
    if (m_framerate != framerate) {
        m_framerate = framerate;
        notify_observers();
    }
}

void Precomposition::set_canvas_background_color(Color4ub color)
{
    if (m_canvas_bg_color != color) {
        m_canvas_bg_color = color;
        notify_observers();
    }
}

void Precomposition::set_show_canvas_bg_color(bool show)
{
    if (m_show_canvas_bg_color != show) {
        m_show_canvas_bg_color = show;
        notify_observers();
    }
}
PropertyGroup *Precomposition::property_group()
{
    return m_prop_group.get();
}

const PropertyGroup *Precomposition::property_group() const
{
    return m_prop_group.get();
}

Layer *Precomposition::search_layer(const std::string &layer_name)
{
    if (layer_name.size()) {
        for (auto &layer : m_layers) {
            if (layer->name() == layer_name) {
                return layer.get();
            }
        }
    }
    return nullptr;
}

TextLayer *Precomposition::search_text_layer(const std::string &layer_name)
{
    auto layer = search_layer(layer_name);
    if (layer) {
        return layer->to_text_layer();
    }
    return nullptr;
}

ShapeLayer *Precomposition::search_shape_layer(const std::string &layer_name)
{
    auto layer = search_layer(layer_name);
    if (layer) {
        return layer->to_shape_layer();
    }
    return nullptr;
}

NullLayer *Precomposition::search_null_layer(const std::string &layer_name)
{
    auto layer = search_layer(layer_name);
    if (layer) {
        return layer->to_null_layer();
    }
    return nullptr;
}

void Precomposition::set_apply_script_on_change(bool enable)
{
    if (m_apply_property_on_change != enable) {
        m_apply_property_on_change = enable;
        notify_observers();
    }
}

void Precomposition::set_apply_script(const std::string &str)
{
    if (str != m_apply_script) {
        m_apply_script = str;
        notify_observers();
    }
}

void Precomposition::on_update(ISubject *subject)
{
    Object::on_update(subject);
}

void Precomposition::apply_scripts_on_layers(script::ExpressionEngineInterface *engine)
{
    m_engine = engine;
    for (auto it = m_layers.rbegin(); it != m_layers.rend(); ++it) {
        auto *precomp = (*it)->to_precomp_layer();
        if (precomp) {
            precomp->apply_scripts_on_layers(engine);
        }
    }
}

KeyFrame *Precomposition::get_keyframe(const std::string &name)
{
    auto *tag = m_tag_manager.tag(name);
    if (tag) {
        auto entities = tag->entities();
        if (!entities.empty()) {
            return static_cast<KeyFrame *>(entities.front());
        }
    }
    return nullptr;
}

const string Precomposition::apply_script() const
{
    return m_apply_script;
}

Composition::Composition(std::string name,
                         std::string version,
                         int width,
                         int height,
                         KeyFrameTime in_point,
                         KeyFrameTime out_point,
                         FrameTimeType framerate)
    : Precomposition(nullptr,
                     name,
                     version,
                     width,
                     height,
                     in_point,
                     out_point,
                     framerate,
                     composition_descriptor())
{}

Composition::~Composition() {}

void Precomposition::set_layers(Layers &layers)
{
    m_layers = std::move(layers);
    for (auto &layer : m_layers) {
        observe(layer.get());
    }
}

Composition::Composition(const Composition &other)
    : Precomposition(nullptr, other)
{
}

Composition *Composition::clone(Object *object) const
{
    return new Composition(*this);
}

bool is_unique_layer_name(const Composition *comp, const std::string &name)
{
    for (const auto &layer : comp->layers()) {
        if (layer->name() == name)
            return false;
    }
    return true;
}

string get_unique_layer_name(const Composition *comp, const std::string &prefix)
{
    int counter = 0;
    while (true) {
        std::string name = prefix + "_" + std::to_string(counter);
        if (is_unique_layer_name(comp, name)) {
            return name;
        }
        ++counter;
    }

    return {};
}
} // namespace alive::model
