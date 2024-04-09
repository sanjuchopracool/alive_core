#ifndef TEXT_LAYER_PROPERTIES_H
#define TEXT_LAYER_PROPERTIES_H

#include <core/model/property/property_macros.h>
#include <core/model/property/property_type.h>
#include <core/model/text_primitives/text_document_property.h>

namespace inae::model {
namespace descriptors {
inline const PropertyDescriptor &text_doc_descriptor()
{
    static const auto text_doc_descriptor = PropertyDescriptor("Source Text",
                                                               PropertyType::e_TextDocument,
                                                               PropertyDescriptor::e_NoFlags);
    return text_doc_descriptor;
}
} // namespace descriptors
#define ADD_TEXT \
    ADD_DYNAMIC_PROPERTY(core::TextDocumentProperty, text, descriptors::text_doc_descriptor())
} // namespace inae::model
#endif // TEXT_LAYER_PROPERTIES_H
