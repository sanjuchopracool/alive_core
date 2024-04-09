#include "object_descriptor.h"
#include "property_type.h"

namespace inae::model {

ObjectDescriptor::ObjectDescriptor(const std::string &name, ObjectType type)
    : PropertyDescriptor(name, PropertyType::e_Object, PropertyDescriptor::e_NoFlags)
    , m_object_type(type)
{}

} // namespace inae::model
