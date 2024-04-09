#include "property.h"
#include "object.h"
namespace inae::model {

Property::Property(Object *object, const PropertyDescriptor &descriptor)
    : TreeItem(ItemType::e_Property, object)
    , m_object(object)
    , m_descriptor(descriptor)
{
    if (object) {
        object->append_item(this);
        object->observe(this);
    }
}

} // namespace inae::model
