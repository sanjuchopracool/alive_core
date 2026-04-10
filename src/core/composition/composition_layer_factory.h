#ifndef ANIMATIONLAYERFACTORY_H
#define ANIMATIONLAYERFACTORY_H

#include <memory>

namespace alive::model {

class Layer;
class LayerNode;
struct PaintBufferInfo;

class CompositionLayerFactory
{
public:
    static std::unique_ptr<LayerNode> composition_layer(Layer *layer);
};

} // namespace alive::model

#endif // ANIMATIONLAYERFACTORY_H
