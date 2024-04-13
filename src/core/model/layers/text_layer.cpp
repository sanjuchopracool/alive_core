#include "text_layer.h"
#include <core/model/property/object_descriptor.h>

namespace inae::model {

const LayerDescriptor &text_layer_descriptor()
{
    static const auto text_layer_descriptor = LayerDescriptor("TextLayer", LayerType::e_Text);
    return text_layer_descriptor;
}

inline const ObjectDescriptor &text_group_descriptor()
{
    static const auto text_group_descriptor = ObjectDescriptor("Text",
                                                               ObjectDescriptor::e_DummyGroup);
    return text_group_descriptor;
}

TextLayer::TextLayer(Object *object, int index)
    : Layer(object, text_layer_descriptor())
    , m_text_group(std::make_unique<Object>(this, text_group_descriptor()))
{
    set_layer_index(index);
    remove_item(&m_text);
    m_text_group->insert_item_at_index(&m_text, 0);
    move_item(1, 0);
}

TextLayer::~TextLayer() {}
} // namespace inae::model
