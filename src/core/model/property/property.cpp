#include <core/model/property/property.h>
#include <core/model/property/object.h>
#include <core/model/property/property_type.h>

namespace alive::model {

Property::Property(Object *object, const PropertyDescriptor &descriptor, const std::string name)
    : TreeItem(ItemType::e_Property, object)
    , m_object(object)
    , m_descriptor(descriptor)
    , m_name(name)
{
    if (object) {
        object->append_item(this);
        object->observe(this);
    }
}

Property::~Property() {}

void Property::set_name(const std::string &name)
{
    if (m_name != name) {
        m_name = name;
        notify_observers();
    }
}

bool Property::linkable() const
{
    return m_descriptor.linkable();
}

bool Property::is_static() const
{
    return (type() >= PropertyType::e_StaticPropertyStart
            && type() < PropertyType::e_StaticPropertyEnd);
}

void Property::set_id(const std::string &id)
{
    if (m_id != id) {
        m_id = id;
        notify_observers();
    }
}

} // namespace alive::model
