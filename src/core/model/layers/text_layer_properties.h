#ifndef TEXT_LAYER_PROPERTIES_H
#define TEXT_LAYER_PROPERTIES_H

#include <core/model/property/property_macros.h>
#include <core/model/property/property_type.h>
#include <core/model/text_primitives/text.h>

namespace alive::model {
namespace descriptors {
inline const PropertyDescriptor &text_doc_descriptor()
{
    static const auto text_doc_descriptor = PropertyDescriptor("Source Text",
                                                               PropertyType::e_TextDocument,
                                                               PropertyDescriptor::e_NoFlags);
    return text_doc_descriptor;
}
} // namespace descriptors
#define ADD_TEXT ADD_DYNAMIC_PROPERTY(core::Text, text, descriptors::text_doc_descriptor())
} // namespace alive::model
#endif // TEXT_LAYER_PROPERTIES_H
