#ifndef PROPERTY_GROUP_H
#define PROPERTY_GROUP_H

#include <Corrade/Containers/Pointer.h>
#include <core/model/property/object.h>
#include <core/serializer/serializer_common.h>
#include <functional>

namespace alive::core {
class Serializer;
}

namespace alive::model {
class PropertyGroup : public Object
{
public:
    using PropertyPtr = Corrade::Containers::Pointer<Property>;
    using itr = std::vector<PropertyPtr>::iterator;
    using c_itr = std::vector<PropertyPtr>::const_iterator;
    PropertyGroup(Object *object = nullptr);
    PropertyGroup(Object *object, const PropertyGroup &other);
    ~PropertyGroup();

    PropertyGroup *clone(Object *object) const override;
    size_t count() const;
    Property *search_with_name(const std::string &name);
    std::pair<Property *, int> search_with_name_from_index(const std::string &name,
                                                           int start_index = 0);

    Property *search_with_id(const std::string &id);
    std::pair<Property *, int> search_with_id_from_index(const std::string &id, int start_index = 0);

    Property *at(size_t index);
    int index_of(Property *property);
    const Property *at(size_t index) const;
    int add(Property *prop, int index = -1);
    int remove(Property *prop);
    Property *remove(size_t index);
    Property *remove(const std::string &name);

    void on_update(ISubject *subject) override;
    itr begin() { return m_props.begin(); };
    itr end() { return m_props.end(); };

    c_itr begin() const { return m_props.begin(); };
    c_itr end() const { return m_props.end(); };

    void assign_values_from_other(const PropertyGroup *other);

private:
    std::pair<Property *, int> search_from_index(std::function<bool(const PropertyPtr &)> func,
                                                 int start_index = 0);
    std::vector<PropertyPtr> m_props;

private:
    ADD_FRIEND_SERIALIZERS
};
} // namespace alive::model

#endif // PROPERTY_GROUP_H
