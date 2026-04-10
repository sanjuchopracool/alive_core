#include "group_node.h"

//#include "model/shape_items/group.h"
//#include "repeater_node.h"
#include "animation_node_factory.h"
#include <algorithm>
#include <core/composition/paint_context.h>
#include <include/core/SkCanvas.h>
#include <core/model/shape_items/group.h>

namespace alive::model {

GroupNode *group_node_from_shape_node(ShapeAnimationNode *node)
{
    if (node && node->shape_item()->type() == ShapeType::e_Group) {
        return static_cast<GroupNode *>(node);
    }
    return nullptr;
}

GroupNode::GroupNode(const Group *group)
    : ShapeAnimationNode(group)
{
    for (const auto &shape_item : group->shapes()) {
        m_nodes.emplace_back(AnimationNodeFactory::node_for_shape_item(shape_item.get()));
        observe(m_nodes.back().get());
    }
}

bool GroupNode::update(UpdateContext &context)
{
    m_dirty = false;
    for (auto it = m_nodes.rbegin(); it != m_nodes.rend(); ++it) {
        (*it)->update(context);
    }
    return m_dirty;
}

void GroupNode::set_content(const std::vector<ShapeAnimationNode *> &items_before,
                            const std::vector<ShapeAnimationNode *> &items_after)
{
    // TODO same in shape_layer
    m_paint_items.clear();
    std::vector<ShapeAnimationNode *> group_items_before(items_before);
    group_items_before.reserve(items_before.size() + m_nodes.size());
    std::vector<ShapeAnimationNode *> group_items_after;
    group_items_after.reserve(m_nodes.size());
    std::transform(m_nodes.cbegin(),
                   m_nodes.cend(),
                   std::back_inserter(group_items_after),
                   [](const auto &it) { return it.get(); });
    while (group_items_after.size()) {
        ShapeAnimationNode *item = group_items_after.back();
        group_items_after.pop_back();
        item->set_content(group_items_before, group_items_after);
        group_items_before.emplace_back(item);

        auto *paint_element = dynamic_cast<PaintingElement *>(item);
        if (paint_element)
            m_paint_items.emplace_back(paint_element);
    }
}

bool GroupNode::add_shape(Group *group, ShapeItem *shape, int shape_index)
{
    if (this->shape_item() == group) {
        return add_shape(shape, shape_index);
    } else {
        for (auto &node : m_nodes) {
            auto group_node = group_node_from_shape_node(node.get());
            if (group_node && group_node->add_shape(group, shape, shape_index)) {
                return true;
            }
        }
    }
    return false;
}

bool GroupNode::remove_shape(Group *group, ShapeItem *shape)
{
    if (this->shape_item() == group) {
        return remove_shape(shape);
    } else {
        for (auto &node : m_nodes) {
            auto group_node = group_node_from_shape_node(node.get());
            if (group_node && group_node->remove_shape(group, shape)) {
                return true;
            }
        }
    }
    return false;
}

bool GroupNode::add_shape(const ShapeItem *shape, int index)
{
    if (shape) {
        assertm(index <= m_nodes.size(), "Invalid Shape Node Index");
        auto node = AnimationNodeFactory::node_for_shape_item(shape);
        if (node) {
            m_nodes.emplace(m_nodes.begin() + index, node.release());
            observe(m_nodes[index].get());
            return true;
        }
    }

    return false;
}

bool GroupNode::remove_shape(ShapeItem *shape)
{
    if (shape) {
        auto it = std ::find_if(m_nodes.begin(), m_nodes.end(), [shape](const auto &u_ptr) {
            return u_ptr->shape_item() == shape;
        });

        if (it != m_nodes.end()) {
            stop_observing(it->get());
            m_nodes.erase(it);
            return true;
        }
    }
    return false;
}

bool GroupNode::move_shapes(int from, int to)
{
    bool result = false;
    if ((from != to) && (from >= 0 && from < m_nodes.size()) && (to >= 0 && to < m_nodes.size())) {
        ShapeAnimationNode *shape = m_nodes[from].release();
        m_nodes.erase(m_nodes.begin() + from);
        m_nodes.emplace(m_nodes.begin() + to, shape);
        result = true;
    }

    return result;
}

void GroupNode::draw(PaintContext &context) const

{
    if (m_shape_item->hidden())
        return;

    context.save();
    for (auto *item : m_paint_items) {
        item->draw(context);

        if (item->is_repeater()) {
            break;
        }
    }
    context.restore();
}

void GroupNode::reset_content()
{
    std::vector<ShapeAnimationNode *> group_items_before;
    std::vector<ShapeAnimationNode *> group_items_after;
    GroupNode::set_content(group_items_after, group_items_after);
}
} // namespace alive::model
