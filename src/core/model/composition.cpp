#include "composition.h"
#include <algorithm>

using namespace std;
namespace inae::model {

const ObjectDescriptor &composition_descriptor()
{
    static const auto composition_descriptor = ObjectDescriptor("Composition",
                                                                ObjectDescriptor::e_Composition);
    return composition_descriptor;
}

Composition::Composition(std::string name,
                         std::string version,
                         int width,
                         int height,
                         KeyFrameTime in_point,
                         KeyFrameTime out_point,
                         FrameTimeType framerate)
    : Object(nullptr, composition_descriptor())
    , m_name(std::move(name))
    , m_version(std::move(version))
    , m_width(width)
    , m_height(height)
    , m_in_point(in_point)
    , m_out_point(out_point)
    , m_framerate(framerate)
{}

Composition::~Composition() {}

void Composition::set_width(int width)
{
    if (m_width != width) {
        m_width = width;
        notify_observers();
    }
}

void Composition::set_height(int height)
{
    if (m_height != height) {
        m_height = height;
        notify_observers();
    }
}

void Composition::set_in_point(KeyFrameTime in_point)
{
    if (m_in_point != in_point) {
        m_in_point = in_point;
        notify_observers();
    }
}

void Composition::set_out_point(KeyFrameTime out_point)
{
    if (m_out_point != out_point) {
        m_out_point = out_point;
        notify_observers();
    }
}

void Composition::set_framerate(FrameTimeType framerate)
{
    if (m_framerate != framerate) {
        m_framerate = framerate;
        notify_observers();
    }
}

void Composition::set_layers(Layers &layers)
{
    m_layers = std::move(layers);
    for (const auto &layer : m_layers) {
        observe(layer.get());
    }
}

int Composition::index_of_layer(const Layer *layer) const
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

void Composition::add_layer_at_index(Layer *layer, int index)
{
    assert(index <= m_layers.size());
    m_layers.emplace(m_layers.begin() + index, layer);
    insert_item_at_index(layer, index);
    notify_observers();
}

Layer *Composition::remove_layer_at_index(int index)
{
    assert(index < m_layers.size());
    Layer *layer = m_layers[index].release();
    m_layers.erase(m_layers.begin() + index);
    remove_item(layer);
    notify_observers();
    return layer;
}

void Composition::remove_matte_for_layers(int matte_index)
{
    for (auto &layer : m_layers) {
        if (layer->matte_layer_index() == matte_index) {
            layer->set_matte_layer_index(-1);
        }
    }
}

bool Composition::move_layer(int from, int to)
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

void Composition::set_name(const std::string &name)
{
    if (m_name != name) {
        m_name = name;
        notify_observers();
    }
}

void Composition::set_uuid(const std::string &uuid)
{
    if (m_uuid != uuid) {
        m_uuid = uuid;
        notify_observers();
    }
}

int Composition::layers_max_index() const
{
    int index = 0;
    for (const auto &layer : m_layers) {
        index = std::max(layer->layer_index(), index);
    }
    return index + 1;
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

} // namespace inae::model
