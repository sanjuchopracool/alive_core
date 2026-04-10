#ifndef OBJECT_H
#define OBJECT_H

#include <core/model/property/object_descriptor.h>
#include <core/model/property/property.h>
#include <core/serializer/serializer_common.h>
#include <string>

namespace alive::model {
class Composition;
class Group;
class Object : public GroupTreeItem, public CascadeUpdateListener
{
    // TODO check if container like interface is required
public:
    Object(Object *parent,
           const ObjectDescriptor &descriptor,
           const std::string &name = {},
           ItemType type = ItemType::e_Object);
    Object(Object *parent, const Object &other);
    virtual ~Object() = default;

    ObjectDescriptor::ObjectType object_type() const;

    //    constexpr PropertyValueType get_property_value_type() { return PropertyValueType::e_Object; }
    virtual bool is_object_visible() const { return true; }
    virtual void set_object_visible(bool) {}
    bool support_visibility() const;
    //    const Composition *composition() const;
    bool is_layer() const;
    bool is_shape_item() const;
    bool is_content_group() const;
    Group *to_shape_group();
    bool can_delete() const;
    bool can_rename() const;

    const std::string &name() const override final;
    void set_name(const std::string &name) override final;

    virtual Object *clone(Object *object) const = 0;

    // Attached Properties will be visible as extra node

protected:
    const std::string &object_name() const;
    const ObjectDescriptor &m_descriptor;

private:
    std::string m_name;

private:
    ADD_FRIEND_SERIALIZERS
};

class DummyObject : public Object
{
public:
    DummyObject(Object *object, const ObjectDescriptor &descriptor);
    DummyObject *clone(Object *object) const override;
};
} // namespace alive::model
#endif // OBJECT_H
