#ifndef PRECOMPOSITIONLAYER_H
#define PRECOMPOSITIONLAYER_H

#include "layer_node.h"
#include <Corrade/Containers/Pointer.h>
#include <vector>

namespace alive {
namespace model {
class Layer;
class Object;
class PrecompositionLayer;

using LayerPtr = Corrade::Containers::Pointer<alive::model::Layer>;
using Layers = std::vector<LayerPtr>;

void load_layers(SimpleObserver *observer,
                 const Layers &layers,
                 std::vector<std::unique_ptr<LayerNode>> &layer_nodes);

class LayerNodesContainer
{
public:
    // scripting support, support only read behavior
    const LayerNode *get_layer(const std::string &layer_name);
    const SolidLayerNode *get_solid_layer(const std::string &layer_name)
    {
        auto layer = get_layer(layer_name);
        if (layer) {
            return layer->to_solid_layer();
        }
        return nullptr;
    }
    const TextLayerNode *get_text_layer(const std::string &layer_name)
    {
        auto layer = get_layer(layer_name);
        if (layer) {
            return layer->to_text_layer();
        }
        return nullptr;
    }
    const ShapeLayerNode *get_shape_layer(const std::string &layer_name)
    {
        auto layer = get_layer(layer_name);
        if (layer) {
            return layer->to_shape_layer();
        }
        return nullptr;
    }

protected:
    std::vector<std::unique_ptr<LayerNode>> m_layers_nodes;
};

class PrecompositionLayerNode : public LayerNode, public LayerNodesContainer
{
public:
    PrecompositionLayerNode(PrecompositionLayer *layer);
    FrameTimeType time() const { return m_last_updated_frame; }

    void on_update(ISubject* s) override;

protected:
    void draw_layer(PaintContext &context) const override;
    void update_layer(UpdateContext &context) override;

private:
    FrameTimeType m_last_updated_frame = -1;
    PrecompositionLayer* m_precomp_layer = nullptr;
};

} // namespace model
} // namespace alive

#endif // PRECOMPOSITIONLAYER_H
