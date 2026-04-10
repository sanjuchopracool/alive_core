#ifndef TEXT_LAYER_H
#define TEXT_LAYER_H

#include "text_layer_properties.h"
#include <core/model/layers/layer.h>

namespace alive::model {

class TextLayer : public Layer
{
    ADD_TEXT
public:
    TextLayer(Object *object, int index = -1);
    ~TextLayer() override;

    TextLayer *clone_layer(Object *object, int index) const override;

private:
    TextLayer(Object *obj, const TextLayer &other, int index);

private:
    std::unique_ptr<DummyObject> m_text_group;
    ADD_FRIEND_SERIALIZERS
};
} // namespace alive::model

#endif // TEXT_LAYER_H
