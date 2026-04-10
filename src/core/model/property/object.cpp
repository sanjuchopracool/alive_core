#include "object.h"
#include "property.h"

#include <core/model/composition.h>
#include <core/model/shape_items/group.h>

namespace alive::model {

Object::Object(Object *parent,
               const ObjectDescriptor &descriptor,
               const std::string &name,
               ItemType type)
    : GroupTreeItem(type, parent)
    , m_descriptor(descriptor)
    , m_name(name)
{
    if (parent) {
        parent->append_item(this);
        parent->observe(this);
    }
}

Object::Object(Object *parent, const Object &other)
    : GroupTreeItem(other.item_type(), parent)
    , m_descriptor(other.m_descriptor)
    , m_name(other.m_name)
{
    if (parent) {
        parent->append_item(this);
        parent->observe(this);
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

bool Object::can_delete() const
{
    return ObjectDescriptor::can_delete(this->object_type());
}

bool Object::can_rename() const
{
    return can_delete();
}

const std::string &Object::name() const
{
    if (m_name.empty()) {
        return Object::object_name();
    }
    return m_name;
}

void Object::set_name(const std::string &name)
{
    if (m_name != name) {
        m_name = name;
        notify_observers();
    }
}

DummyObject::DummyObject(Object *object, const ObjectDescriptor &descriptor)
    : Object(object, descriptor)
{}

DummyObject *DummyObject::clone(Object *object) const
{
    return new DummyObject(object, m_descriptor);
}

} // namespace alive::model
