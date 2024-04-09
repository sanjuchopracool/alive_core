#include "tree_item.h"
#include <core/model/layers/layer.h>
#include <core/model/property/object.h>
#include <core/model/property/property.h>
#include <algorithm>

namespace inae {
int GroupTreeItem::index_of_item(const TreeItem *p) const
{
    auto it = std::find(m_items.cbegin(), m_items.cend(), p);
    if (it != m_items.cend()) {
        return static_cast<int>(it - m_items.cbegin());
    }

    return -1;
}

const TreeItem *GroupTreeItem::item_at(int index) const
{
    return const_cast<GroupTreeItem *>(this)->item_at(index);
}

TreeItem *GroupTreeItem::item_at(int index)
{
    if (index >= 0 && index < m_items.size()) {
        return m_items.at(index);
    }
    return nullptr;
}

bool GroupTreeItem::append_item(TreeItem *item)
{
    return insert_item_at_index(item, m_items.size());
}

bool GroupTreeItem::insert_item_at_index(TreeItem *item, int index)
{
    if ((index <= m_items.size())
        && (std::find(m_items.cbegin(), m_items.cend(), item) == m_items.end())) {
        m_items.emplace(m_items.begin() + index, item);
        if (item->parent() != this) {
            item->set_parent(this);
        }
        return true;
    }
    return false;
}

bool GroupTreeItem::remove_item(TreeItem *item)
{
    auto it = std::remove(m_items.begin(), m_items.end(), item);
    bool removed = it != m_items.end();
    m_items.erase(it, m_items.end());
    if (removed) {
        item->set_parent(nullptr);
    }
    return removed;
}

bool GroupTreeItem::move_item(int from, int to)
{
    if ((from != to) && (from >= 0 && from < m_items.size()) && (to >= 0 && to < m_items.size())) {
        TreeItem *item = m_items[from];
        m_items.erase(m_items.begin() + from);
        m_items.emplace(m_items.begin() + to, item);
        return true;
    }

    return false;
}

int GroupTreeItem::item_count() const
{
    return static_cast<int>(m_items.size());
}

const model::Property *TreeItem::to_property() const
{
    return const_cast<TreeItem *>(this)->to_property();
}

model::Property *TreeItem::to_property()
{
    if (m_type == ItemType::e_Property) {
        return static_cast<model::Property *>(this);
    }
    return nullptr;
}

const model::Object *TreeItem::to_object() const
{
    return const_cast<TreeItem *>(this)->to_object();
}

model::Object *TreeItem::to_object()
{
    if (m_type == ItemType::e_Object || m_type == ItemType::e_Layer) {
        return static_cast<model::Object *>(this);
    }
    return nullptr;
}

const model::Layer *TreeItem::to_layer() const
{
    return const_cast<TreeItem *>(this)->to_layer();
}

model::Layer *TreeItem::to_layer()
{
    if (m_type == ItemType::e_Layer) {
        return static_cast<model::Layer *>(this);
    }
    return nullptr;
}

} // namespace inae
