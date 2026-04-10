#include "shape_layer.h"

namespace alive::model {

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

ShapeLayer *ShapeLayer::clone_layer(Object *obj, int index) const
{
    return new ShapeLayer(obj, *this, index);
}

Rectangle *ShapeLayer::search_rect(const std::string &name)
{
    return m_group->search_rect(name);
}

Fill *ShapeLayer::search_fill(const std::string &name)
{
    return m_group->search_fill(name);
}

Stroke *ShapeLayer::search_stroke(const std::string &name)
{
    return m_group->search_stroke(name);
}

ShapeLayer::ShapeLayer(Object *object, const ShapeLayer &other, int index)
    : Layer(object, other)
    , m_group(other.m_group->clone_shape_item(this))
{
    set_layer_index(index);
    remove_item(m_group.get());
    insert_item_at_index(m_group.get(), 0);
    observe(m_group.get());
}
} // namespace alive::model
