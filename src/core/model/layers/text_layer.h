#ifndef TEXT_LAYER_H
#define TEXT_LAYER_H

#include "text_layer_properties.h"
#include <include/core/SkColor.h>
#include <core/model/layers/layer.h>

namespace inae::model {

class TextLayer : public Layer
{
    ADD_TEXT
public:
    TextLayer(Object *object, int index = -1);
    ~TextLayer() override;

private:
    std::unique_ptr<Object> m_text_group;
    friend class inae::core::Serializer;
};
} // namespace inae::model

#endif // TEXT_LAYER_H
