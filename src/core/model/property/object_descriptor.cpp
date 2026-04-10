#include "object_descriptor.h"

namespace alive {
namespace model {

ObjectDescriptor::ObjectDescriptor(const std::string &name, ObjectType type)
    : m_name(name)
    , m_object_type(type)
{}

bool ObjectDescriptor::can_delete(ObjectType type)
{
    return !(type == ObjectType::e_ContentGroup || type == ObjectType::e_DummyGroup
             || type == ObjectType::e_Transform);
}

} // namespace model
} // namespace alive
