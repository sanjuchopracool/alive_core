#ifndef COMPOSITION_NODE_H
#define COMPOSITION_NODE_H

#include <memory>
#include <vector>

#include "layer_node.h"
#include "precomposition_layer_node.h"
#include <core/model/property/update_listener.h>

class SkCanvas;

namespace alive::script {
class ExpressionEngine;
class ExpressionEngineInterface;
} // namespace alive::script

namespace alive {
class CanvasBuffer;
}

namespace alive::model {
class Composition;
class ShapeLayer;
class ShapeItem;
class Object;
class Group;

class CompositionNode : public CascadeUpdateListener, public LayerNodesContainer
{
public:
    CompositionNode(const model::Composition *animation, bool create_playground_engine = false);
    ~CompositionNode();

    void draw(SkCanvas *canvas);
    bool update(FrameTimeType t, bool force_update);

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

    script::ExpressionEngineInterface *property_engine() const;

    const Composition *composition() const { return m_animation; }

    Color4ub canvas_background_color() const;
    const FrameTimeType &time() const { return m_last_updated_frame; }
    FrameTimeType fps() const;

private:
    void load_layers();

    LayerNode *layer_from_layer_index(int index) const;

private:
    const Composition *m_animation;
    FrameTimeType m_last_updated_frame = -1;

    std::unique_ptr<script::ExpressionEngine> m_expr_engine;
};

class RasterCompositionNode : public CompositionNode
{
public:
    RasterCompositionNode(const model::Composition *animation, bool create_playground_engine = false);
    ~RasterCompositionNode();

    void resize(int32_t width, int32_t height);
    const uint8_t *draw_data();

private:
    Corrade::Containers::Pointer<CanvasBuffer> m_buffer;
};

} // namespace alive::model

#endif // COMPOSITION_NODE_H
