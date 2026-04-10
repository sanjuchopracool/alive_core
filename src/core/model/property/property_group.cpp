#include "property_group.h"
#include "property.h"
#include <algorithm>

namespace alive::model {

const ObjectDescriptor &prop_group_descriptor()
{
    static auto prop_group_descriptor = ObjectDescriptor("Properties",
                                                         ObjectDescriptor::e_PropertyGroup);
    prop_group_descriptor.set_support_visibility(false);
    return prop_group_descriptor;
}

PropertyGroup::PropertyGroup(Object *object)
    : Object(object, prop_group_descriptor())
{}

PropertyGroup::PropertyGroup(Object *object, const PropertyGroup &other)
    : Object(object, prop_group_descriptor(), other.name())
{
    for (auto &item : other.m_props) {
        add(item->clone(nullptr));
    }
}

PropertyGroup::~PropertyGroup() {}

PropertyGroup *PropertyGroup::clone(Object *object) const
{
    return new PropertyGroup(object, *this);
}

size_t PropertyGroup::count() const
{
    return m_props.size();
}

Property *PropertyGroup::search_with_name(const std::string &name)
{
    return search_with_name_from_index(name).first;
}

std::pair<Property *, int> PropertyGroup::search_with_name_from_index(const std::string &name,
                                                                      int start_index)
{
    return search_from_index([name](const PropertyPtr &ptr) { return ptr->name() == name; },
                             start_index);
}

Property *PropertyGroup::search_with_id(const std::string &id)
{
    return search_with_id_from_index(id).first;
}

std::pair<Property *, int> PropertyGroup::search_with_id_from_index(const std::string &id,
                                                                    int start_index)
{
    return search_from_index([id](const PropertyPtr &ptr) { return ptr->id() == id; }, start_index);
}

Property *PropertyGroup::at(size_t index)
{
    if (index < m_props.size()) {
        return (m_props.begin() + index)->get();
    }
    return nullptr;
}

int PropertyGroup::index_of(Property *property)
{
    return search_from_index([property](const PropertyPtr &ptr) { return ptr.get() == property; })
        .second;
}

const Property *PropertyGroup::at(size_t index) const
{
    if (index < m_props.size()) {
        return (m_props.begin() + index)->get();
    }
    return nullptr;
}

int PropertyGroup::add(Property *prop, int index)
{
    if (index == -1) {
        index = m_props.size();
    }
    assert(index <= m_props.size());
    m_props.emplace(m_props.begin() + index, prop);
    insert_item_at_index(prop, index);
    notify_observers();
    observe(prop);

    return index;
}

int PropertyGroup::remove(Property *property)
{
    auto it = std::find_if(m_props.begin(), m_props.end(), [property](auto &prop) {
        return prop.get() == property;
    });

    if (it != m_props.end()) {
        int index = it - m_props.begin();
        remove(index);
        return index;
    }

    return -1;
}

Property *PropertyGroup::remove(size_t index)
{
    assert(index < m_props.size());
    Property *prop = m_props[index].release();
    m_props.erase(m_props.begin() + index);
    remove_item(prop);
    notify_observers();
    return prop;
}

Property *PropertyGroup::remove(const std::string &name)
{
    auto it = std::find_if(m_props.begin(), m_props.end(), [name](auto &prop) {
        return prop->name() == name;
    });

    if (it != m_props.end()) {
        return remove(it - m_props.begin());
    }
    return nullptr;
}

void PropertyGroup::on_update(ISubject *subject)
{
    Object::on_update(this);
    mark_dirty(false);
}

void PropertyGroup::assign_values_from_other(const PropertyGroup *other)
{
    for (const auto &property : *other) {
        auto *p = search_with_id(property->id());
        if (p) {
            p->assign_value_from(property.get());
        }
    }
}

std::pair<Property *, int> PropertyGroup::search_from_index(
    std::function<bool(const PropertyPtr &)> func, int start_index)
{
    if (start_index < 0) {
        start_index = 0;
    }

    auto it = std::find_if(m_props.begin() + start_index, m_props.end(), func);

    if (it != m_props.end()) {
        return std::make_pair(it->get(), static_cast<int>(it - m_props.begin()));
    }

    return std::make_pair(nullptr, -1);
}

} // namespace alive::model
