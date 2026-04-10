#include "precomposition_layer_node.h"
#include <core/composition/animation_nodes/animation_node.h>
#include <core/composition/composition_layer_factory.h>
#include <core/model/layers/layer.h>
#include <core/model/layers/precomposition_layer.h>
#include <core/script/expression_engine.h>
#include <unordered_map>

namespace alive::model {
PrecompositionLayerNode::PrecompositionLayerNode(PrecompositionLayer *layer)
    : LayerNode(layer)
    , m_precomp_layer(layer)
{
    alive::model::load_layers(this, layer->layers(), m_layers_nodes);
}

void PrecompositionLayerNode::on_update(ISubject *s)
{
    if (!m_precomp_layer->is_applying_script()) {
        CascadeUpdateListener::on_update(s);
    }
}

void PrecompositionLayerNode::draw_layer(PaintContext &context) const
{
    for (auto it = m_layers_nodes.rbegin(); it != m_layers_nodes.rend(); ++it)
        (*it)->draw(context);
}

void load_layers(SimpleObserver *observer,
                 const Layers &layers,
                 std::vector<std::unique_ptr<LayerNode>> &layer_nodes)
{
    std::unordered_map<int, int> layer_index_to_local_index;
    int i = 0;
    for (auto &layer : layers) {
        layer_nodes.emplace_back(
            CompositionLayerFactory::composition_layer(const_cast<model::Layer *>(layer.get())));
        observer->observe(layer_nodes.back().get());
        layer_index_to_local_index[layer->layer_index()] = i;
        i++;
    }

    for (int i = 0; i < layer_nodes.size(); ++i) {
        // set parent layer
        int parent_layer_index = layer_nodes[i]->parent_layer_index();
        if (parent_layer_index != -1) {
            auto it = layer_index_to_local_index.find(parent_layer_index);
            if (it != layer_index_to_local_index.end()) {
                layer_nodes[i]->set_parent_layer(layer_nodes[it->second].get());
            }
        }
        // set alpha matter layer
        int matte_layer_index = layer_nodes[i]->matte_layer_index();
        if (matte_layer_index != -1) {
            auto it = layer_index_to_local_index.find(matte_layer_index);
            if (it != layer_index_to_local_index.end()) {
                layer_nodes[i]->set_matte_layer(layer_nodes[it->second].get());
            }
        }
    }
}

void PrecompositionLayerNode::update_layer(UpdateContext &context)
{
    context.expr_engine.update_pre_comp(*this);
    LayerNode::update_base_layer_node(context);
    m_last_updated_frame = context.time;
    UpdateContext new_context(context.time - m_layer->in_point(),
                              context.force_update,
                              context.expr_engine);
    for (const auto &layer : m_layers_nodes) {
        layer->update_layer(new_context);
    }
    context.expr_engine.reset_comp_node();
}

const LayerNode *LayerNodesContainer::get_layer(const std::string &layer_name)
{
    if (layer_name.size()) {
        for (const auto &layer : m_layers_nodes) {
            if (layer->name() == layer_name) {
                return layer.get();
            }
        }
    }
    return nullptr;
}

} // namespace alive::model
