#ifndef LAYER_NODE_H
#define LAYER_NODE_H

#include <core/alive_types/common_types.h>
#include <core/composition/interfaces/painting_element.h>
#include <core/model/property/update_listener.h>
#include <memory>

class SkMatrix;
namespace alive::model {
class Layer;

class UpdateContext;
class SolidLayerNode;
class TextLayerNode;
class ShapeLayerNode;

class LayerTransformationNode;
class LayerNode : virtual public PaintingElement, virtual public CascadeUpdateListener
{
public:
    LayerNode(Layer *layer);
    ~LayerNode() override;

    FrameTimeType local_frame(FrameTimeType t) const;
    void draw(PaintContext &context) const override;

    virtual void draw_layer(PaintContext &context) const = 0;
    virtual void update_layer(UpdateContext &context) = 0;

    void get_transform(SkMatrix &o_transform) const;
    const LayerTransformationNode *transform_node() { return m_transform.get(); }
    int opacity() const;

    void set_parent_layer(const LayerNode *layer)
    {
        if (layer != this) {
            m_parent_layer = layer;
        }
    }

    const LayerNode *parent_layer() const { return m_parent_layer; }

    const std::string &name() const;
    void set_debug_visible(bool visible);

    const model::Layer *layer() const { return m_layer; }
    model::LayerType layer_type() const;

    const LayerNode *matte_layer() const { return m_matte_layer; }

    void set_matte_layer(const LayerNode *layer)
    {
        if (layer != this) {
            m_matte_layer = layer;
        }
    }

    int layer_index() const;
    int matte_layer_index() const;
    int parent_layer_index() const;
    bool has_matte() const;

    SolidLayerNode *to_solid_layer();
    TextLayerNode *to_text_layer();
    ShapeLayerNode *to_shape_layer();
    const SolidLayerNode *to_solid_layer() const;
    const TextLayerNode *to_text_layer() const;
    const ShapeLayerNode *to_shape_layer() const;

private:
    void draw_nested_layer(PaintContext &context) const;
    void draw_matte_layer(PaintContext &context) const;
    void draw_nested_matte_layers(PaintContext &context) const;

protected:
    void update_base_layer_node(UpdateContext &context);

protected:
    const model::Layer *m_layer;
    const LayerNode *m_matte_layer = nullptr;
    std::unique_ptr<LayerTransformationNode> m_transform;
    const LayerNode *m_parent_layer = nullptr;
    bool m_visible = false;
    bool m_debug_visible = true;
};

} // namespace alive::model

#endif // LAYER_NODE_H
