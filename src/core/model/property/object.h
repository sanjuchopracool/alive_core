#ifndef OBJECT_H
#define OBJECT_H

#include "object_descriptor.h"
#include "property.h"

namespace inae::model {
class Composition;
class Group;
class Object : public GroupTreeItem, public CascadeUpdateListener
{
    // TODO check if container like interface is required
public:
    Object(Object *object, const ObjectDescriptor &descriptor, ItemType type = ItemType::e_Object);
    virtual ~Object() = default;

    ObjectDescriptor::ObjectType object_type() const;
    virtual const std::string &object_name() const;

    //    constexpr PropertyValueType get_property_value_type() { return PropertyValueType::e_Object; }
    virtual bool is_object_visible() const { return true; }
    virtual void set_object_visible(bool) {}
    bool support_visibility() const;
    //    const Composition *composition() const;
    bool is_layer() const;
    bool is_shape_item() const;
    bool is_content_group() const;
    Group *to_shape_group();

protected:
    const ObjectDescriptor &m_descriptor;
};

} // namespace inae::model
#endif // OBJECT_H
