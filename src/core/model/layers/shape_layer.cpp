#include "shape_layer.h"

namespace inae::model {

const LayerDescriptor &shape_layer_descriptor()
{
    static const auto shape_layer_descriptor = LayerDescriptor("ShapeLayer", LayerType::e_Shape);
    return shape_layer_descriptor;
}

const ObjectDescriptor &content_group_descriptor()
{
    static auto group_descriptor = ObjectDescriptor("Contents", ObjectDescriptor::e_ContentGroup);
    group_descriptor.set_support_visibility(false);
    return group_descriptor;
}

ShapeLayer::ShapeLayer(Object *object, int index)
    : Layer(object, shape_layer_descriptor())
    , m_group(std::make_unique<Group>(content_group_descriptor()))
{
    set_layer_index(index);
    insert_item_at_index(m_group.get(), 0);
    observe(m_group.get());
}

ShapeLayer::~ShapeLayer()
{
}

void ShapeLayer::add_shape_item_at_index(ShapeItem *shape, int index)
{
    m_group->add_shape_item_at_index(shape, index);
}

ShapeItem *ShapeLayer::remove_shape_item_at_index(int index)
{
    return m_group->remove_shape_item_at_index(index);
}

bool ShapeLayer::move_shapes(Object *group, int from, int to)
{
    if (group == m_group.get()) {
        return m_group->move_shapes(from, to);
    }
    return false;
}

const Group *ShapeLayer::group() const
{
    return m_group.get();
}

const ShapeItems &ShapeLayer::shapes() const
{
    return m_group->shapes();
}

int ShapeLayer::count() const
{
    return m_group->count();
}

//void ShapeLayer::decode(QJsonObject &in_obj, QList<QString> &out_messages)
//{
//    auto shapes= in_obj.take(shapes_key).toArray();
//    m_shapes = ShapeFactory::shapes_from_object(shapes, out_messages);
//    Layer::decode(in_obj, out_messages);
//}
} // namespace inae::model
