#include "composition_node.h"
#include "composition_layer_factory.h"
#include <core/model/composition.h>
#include <core/model/layers/layer.h>

#include <algorithm>
#include <core/composition/animation_nodes/animation_node.h>
#include <core/composition/paint_context.h>
#include <core/composition/shape_layer_node.h>
#include <core/composition/solid_layer_node.h>
#include <include/core/SkCanvas.h>
#include <core/model/layers/shape_layer.h>
#include <core/script/expression_engine.h>
#include <unordered_map>

namespace inae::model {

CompositionNode::CompositionNode(const model::Composition *animation)
    : m_animation(animation)
    , m_expr_engine(std::make_unique<script::ExpressionEngine>(*this))
{
    load_layers();
}

CompositionNode::~CompositionNode() {}

void CompositionNode::draw(SkCanvas *canvas)
{
    PaintContext ctx{*canvas, 255};
    canvas->concat(SkMatrix::Scale(m_scale_x, m_scale_y));
    for (auto it = m_layers_nodes.rbegin(); it != m_layers_nodes.rend(); ++it)
        (*it)->draw(ctx);
}

bool CompositionNode::update(FrameTimeType t, bool force_update)
{
    m_dirty = false;
    m_last_updated_frame = t;
    UpdateContext context(t, force_update, *m_expr_engine);
    for (const auto &layer : m_layers_nodes) {
        layer->update_layer(context);
    }
    return m_dirty;
}

void CompositionNode::resize_based_on_window(int x, int y)
{
    SkScalar width = m_animation->width();
    SkScalar height = m_animation->height();

    SkScalar h = (height / width) * x;
    SkScalar w = 0;
    SkScalar scale;
    if (h > y) {
        h = y;
        scale = h / height;
        w = (width / height) * y;
    } else {
        w = x;
        scale = w / width;
    }
    m_size = SkSize::Make(w, h);
    m_scale_x = scale;
    m_scale_y = scale;
}

void CompositionNode::slot_new_layer_added(Layer *layer, int index)
{
    if (layer) {
        assertm(index <= m_layers_nodes.size(), "Invalid Layer Node Index");
        auto layer_node = CompositionLayerFactory::composition_layer(layer);
        if (layer_node) {
            m_layers_nodes.emplace(m_layers_nodes.begin() + index, layer_node.release());
            observe(m_layers_nodes[index].get());
        }
    }
}

void CompositionNode::slot_about_to_remove_layer(Layer *layer)
{
    if (layer) {
        auto it = std ::find_if(m_layers_nodes.begin(),
                                m_layers_nodes.end(),
                                [layer](const auto &u_ptr) { return u_ptr->layer() == layer; });

        if (it != m_layers_nodes.end()) {
            // remove matte
            for (auto &other : m_layers_nodes) {
                if (other->matte_layer() == it->get()) {
                    other->set_matte_layer(nullptr);
                }
            }

            stop_observing(it->get());
            m_layers_nodes.erase(it);
        }
    }
}

void CompositionNode::slot_layer_index_changed(Layer *layer, int index, bool is_matte)
{
    if (layer) {
        auto layer_node = layer_from_layer_index(layer->layer_index());
        auto layer_to_set = layer_from_layer_index(index);
        if (layer_node) {
            if (is_matte) {
                layer_node->set_matte_layer(layer_to_set);
            } else {
                layer_node->set_parent_layer(layer_to_set);
            }
        }
    }
}

void CompositionNode::slot_new_shape_added(ShapeLayer *shape_layer,
                                           Group *group,
                                           ShapeItem *shape,
                                           int shape_index)
{
    if (shape_layer) {
        for (const auto &layer : m_layers_nodes) {
            if (layer->layer() == shape_layer) {
                assert(shape_layer->layer_type() == LayerType::e_Shape);
                static_cast<ShapeLayerNode *>(layer.get())->on_shape_added(group, shape, shape_index);
            }
        }
    }
}

void CompositionNode::slot_about_to_remove_shape(ShapeLayer *shape_layer,
                                                 Group *group,
                                                 ShapeItem *shape)
{
    if (shape_layer) {
        for (const auto &layer : m_layers_nodes) {
            if (layer->layer() == shape_layer) {
                assert(shape_layer->layer_type() == LayerType::e_Shape);
                static_cast<ShapeLayerNode *>(layer.get())->on_shape_removed(group, shape);
            }
        }
    }
}

bool CompositionNode::slot_move_shapes(ShapeLayer *shape_layer, Object *group, int from, int to)
{
    if (shape_layer) {
        for (const auto &layer : m_layers_nodes) {
            if (layer->layer() == shape_layer) {
                assert(shape_layer->layer_type() == LayerType::e_Shape);
                return static_cast<ShapeLayerNode *>(layer.get())->on_shapes_moved(group, from, to);
            }
        }
    }
    return false;
}

bool CompositionNode::slot_move_layer(int from, int to)
{
    bool result = false;
    if ((from != to) && (from >= 0 && from < m_layers_nodes.size())
        && (to >= 0 && to < m_layers_nodes.size())) {
        std::swap(m_layers_nodes[from], m_layers_nodes[to]);
    }

    return result;
}

const LayerNode *CompositionNode::get_layer(const std::string &layer_name)
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

script::ExpressionEngineInterface *CompositionNode::engine()
{
    return m_expr_engine.get();
}

void CompositionNode::load_layers()
{
    std::unordered_map<int, int> layer_index_to_local_index;
    int i = 0;
    for (const auto &layer : m_animation->layers()) {
        m_layers_nodes.emplace_back(CompositionLayerFactory::composition_layer(layer.get()));
        observe(m_layers_nodes.back().get());
        layer_index_to_local_index[layer->layer_index()] = i;
        i++;
    }

    for (int i = 0; i < m_layers_nodes.size(); ++i) {
        // set parent layer
        int parent_layer_index = m_layers_nodes[i]->parent_layer_index();
        if (parent_layer_index != -1) {
            auto it = layer_index_to_local_index.find(parent_layer_index);
            if (it != layer_index_to_local_index.end()) {
                m_layers_nodes[i]->set_parent_layer(m_layers_nodes[it->second].get());
            }
        }
        // set alpha matter layer
        int matte_layer_index = m_layers_nodes[i]->matte_layer_index();
        if (matte_layer_index != -1) {
            auto it = layer_index_to_local_index.find(matte_layer_index);
            if (it != layer_index_to_local_index.end()) {
                m_layers_nodes[i]->set_matte_layer(m_layers_nodes[it->second].get());
            }
        }
    }
}

LayerNode *CompositionNode::layer_from_layer_index(int index) const
{
    if (index == -1) {
        return nullptr;
    }

    for (const auto &ptr : m_layers_nodes) {
        if (index == ptr->layer_index()) {
            return ptr.get();
        }
    }
    return nullptr;
}

} // namespace inae::model
