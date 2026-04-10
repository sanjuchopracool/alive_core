#ifndef NULLLAYER_H
#define NULLLAYER_H

#include <core/model/layers/layer.h>

namespace alive::model {

class NullLayer : public Layer
{
public:
    NullLayer(Object *object, int index = -1);

    NullLayer *clone_layer(Object *object, int index) const override;

private:
    NullLayer(Object *object, const NullLayer &other, int index);
};
} // namespace alive::model

#endif // NULLLAYER_H
