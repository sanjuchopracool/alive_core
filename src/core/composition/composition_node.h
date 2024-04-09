#ifndef COMPOSITION_NODE_H
#define COMPOSITION_NODE_H

#include <memory>
#include <vector>

#include "layer_node.h"
#include <core/model/property/update_listener.h>

class SkCanvas;

namespace inae::script {
class ExpressionEngine;
class ExpressionEngineInterface;
}

namespace inae::model {
class Composition;
class ShapeLayer;
class ShapeItem;
class Object;
class Group;
class CompositionNode : public CascadeUpdateListener
{
public:
    CompositionNode(const model::Composition *animation);
    ~CompositionNode();

    void draw(SkCanvas *canvas);
    bool update(FrameTimeType t, bool force_update);

    void resize_based_on_window(int x, int y);
    const SkSize &size() const { return m_size; }
    const std::vector<std::unique_ptr<LayerNode>> &layers() const { return m_layers_nodes; }

    // changes from document
    void slot_new_layer_added(Layer *layer, int index);
    void slot_about_to_remove_layer(Layer *layer);
    void slot_layer_index_changed(Layer *layer, int index, bool is_matte);

    void slot_new_shape_added(ShapeLayer *shape_layer,
                              Group *group,
                              ShapeItem *shape,
                              int shape_index = -1);
    void slot_about_to_remove_shape(ShapeLayer *shape_layer, Group *group, ShapeItem *shape);
    bool slot_move_shapes(ShapeLayer *shape_layer, model::Object *group, int from, int to);
    bool slot_move_layer(int from, int to);

    // scripting support, support only read behavior
    const LayerNode *get_layer(const std::string &layer_name);
    const SolidLayerNode *get_solid_layer(const std::string &layer_name)
    {
        return get_layer(layer_name)->to_solid_layer();
    }
    const TextLayerNode *get_text_layer(const std::string &layer_name)
    {
        return get_layer(layer_name)->to_text_layer();
    }
    const ShapeLayerNode *get_shape_layer(const std::string &layer_name)
    {
        return get_layer(layer_name)->to_shape_layer();
    }

    script::ExpressionEngineInterface *engine();

private:
    void load_layers();

    LayerNode *layer_from_layer_index(int index) const;

private:
    const Composition *m_animation;
    FrameTimeType m_last_updated_frame = -1;

    // layers
    std::vector<std::unique_ptr<LayerNode>> m_layers_nodes;

    // scaling information to fit into rendering canvas (canvas)
    SkSize m_size;
    SkScalar m_scale_x = 1.0f;
    SkScalar m_scale_y = 1.0f;
    std::unique_ptr<script::ExpressionEngine> m_expr_engine;
};
} // namespace inae::model

#endif // COMPOSITION_NODE_H
