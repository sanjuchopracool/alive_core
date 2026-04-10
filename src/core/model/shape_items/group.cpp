#include "group.h"
#include "fill.h"
#include "rectangle.h"
#include "stroke.h"

namespace alive::model {

template<typename T>
T *to_type(ShapeItem *p)
{
    return static_cast<T *>(p);
}
//const QString it_key("it");
const ObjectDescriptor &group_descriptor()
{
    static const auto group_descriptor = ObjectDescriptor("Group", ObjectDescriptor::e_Group);
    return group_descriptor;
}

Group::Group(const ObjectDescriptor &descriptor, Object *object)
    : ShapeItem(ShapeType::e_Group, object, descriptor)
{
}

Group::Group(Object *object, const Group &other)
    : ShapeItem(object, other)
{
    for (const auto &item : other.m_shapes) {
        m_shapes.emplace_back(item->clone_shape_item(this));
    }
}

Group::~Group() {}

void Group::add_shape_item_at_index(ShapeItem *shape, int index)
{
    assert(index <= m_shapes.size());
    m_shapes.emplace(m_shapes.begin() + index, shape);
    insert_item_at_index(shape, index);
    observe(shape);
    notify_observers();
}

ShapeItem *Group::remove_shape_item_at_index(int index)
{
    assert(index < m_shapes.size());
    ShapeItem *shape = m_shapes[index].release();
    m_shapes.erase(m_shapes.begin() + index);
    remove_item(shape);
    stop_observing(shape);
    notify_observers();
    return shape;
}

bool Group::move_shapes(int from, int to)
{
    bool result = false;
    if ((from != to) && (from >= 0 && from < m_shapes.size()) && (to >= 0 && to < m_shapes.size())) {
        ShapeItem *shape = m_shapes[from].release();
        m_shapes.erase(m_shapes.begin() + from);
        m_shapes.emplace(m_shapes.begin() + to, shape);
        result = move_item(from, to);
        notify_observers();
    }

    return result;
}

Group *Group::clone_shape_item(Object *object) const
{
    return new Group(object, *this);
}

Rectangle *Group::search_rect(const std::string &name)
{
    return search_type<Rectangle>(ShapeType::e_Rectangle, name);
}

Fill *Group::search_fill(const std::string &name)
{
    return search_type<Fill>(ShapeType::e_Fill, name);
}

Stroke *Group::search_stroke(const std::string &name)
{
    return search_type<Stroke>(ShapeType::e_Stroke, name);
}

template<typename T>
T *Group::search_type(ShapeType type, const std::string &name)
{
    for (auto &item : m_shapes) {
        if (item->type() == type and item->name() == name) {
            return static_cast<T *>(item.get());
        }
    }
    return nullptr;
}

} // namespace alive::model
