#include "group.h"
//#include "shape_factory.h"

namespace inae::model {

//const QString it_key("it");
const ObjectDescriptor &group_descriptor()
{
    static const auto group_descriptor = ObjectDescriptor("Group", ObjectDescriptor::e_Group);
    return group_descriptor;
}

Group::Group(const ObjectDescriptor &descriptor, Object *object)
    : ShapeItem(ShapeType::Group, object, descriptor)
{
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

//void Group::decode(QJsonObject &in_obj, QList<QString>& out_messages)
//{
//    auto shapes= in_obj.take(it_key).toArray();
//    m_shapes = ShapeFactory::shapes_from_object(shapes, out_messages);
//    ShapeItem::decode(in_obj, out_messages);
//}
} // namespace inae::model
