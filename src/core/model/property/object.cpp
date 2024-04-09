#include "object.h"
#include "property.h"

#include <core/model/composition.h>
#include <core/model/shape_items/group.h>

namespace inae::model {

Object::Object(Object *object, const ObjectDescriptor &descriptor, ItemType type)
    : GroupTreeItem(type, object)
    , m_descriptor(descriptor)
{
    if (object) {
        object->append_item(this);
        object->observe(this);
    }
}

ObjectDescriptor::ObjectType Object::object_type() const
{
    return m_descriptor.object_type();
}

const std::string &Object::object_name() const
{
    return m_descriptor.get_name();
}

bool Object::support_visibility() const
{
    return m_descriptor.support_visibility();
}

//const Composition *Object::composition() const
//{
//    const Object *obj = this;
//    while (obj->object_type() != ObjectDescriptor::e_Composition) {
//        obj = obj->parent();
//    }
//    return static_cast<const Composition *>(obj);
//}

bool Object::is_layer() const
{
    return m_descriptor.object_type() == ObjectDescriptor::e_Layer;
}

bool Object::is_shape_item() const
{
    auto obj_type = m_descriptor.object_type();
    return obj_type > ObjectDescriptor::e_ShapeItemStart
           && obj_type < ObjectDescriptor::e_ShapeItemEnd;
}

bool Object::is_content_group() const
{
    return m_descriptor.object_type() == ObjectDescriptor::e_ContentGroup;
}

Group *Object::to_shape_group()
{
    if (this->object_type() == ObjectDescriptor::e_ContentGroup
        || this->object_type() == ObjectDescriptor::e_Group) {
        return static_cast<model::Group *>(this);
    }

    return nullptr;
}
} // namespace inae::model
