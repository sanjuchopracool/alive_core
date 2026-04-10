#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <string>
#include <vector>

namespace alive {
class GroupTreeItem;

namespace model {
class Property;
class Object;
class Layer;
} // namespace model

class TreeItem
{
public:
    enum class ItemType { e_Property, e_Object, e_Layer };
    TreeItem(ItemType type = ItemType::e_Property, const GroupTreeItem *parent = nullptr)
        : m_type(type)
        , m_parent(parent)
    {}
    virtual ~TreeItem() = default;
    virtual int index_of_item(const TreeItem *p) const { return -1; }
    virtual TreeItem *item_at(int index) { return nullptr; }
    virtual const TreeItem *item_at(int index) const { return nullptr; }
    virtual bool append_item(TreeItem *item) { return false; }
    virtual bool insert_item_at_index(TreeItem *item, int index) { return false; }
    virtual bool remove_item(TreeItem *item) { return false; }
    virtual bool move_item(int from, int to) { return false; }
    virtual int item_count() const { return 0; }
    virtual bool is_group() const { return false; }

    const GroupTreeItem *parent() const { return m_parent; }
    void set_parent(const GroupTreeItem *parent) { m_parent = parent; }
    ItemType item_type() const { return m_type; }

    const model::Property *to_property() const;
    model::Property *to_property();

    const model::Object *to_object() const;
    model::Object *to_object();

    const model::Layer *to_layer() const;
    model::Layer *to_layer();

    // property support
    virtual const std::string &name() const = 0;
    virtual void set_name(const std::string &name) = 0;

    std::string path() const;

private:
    const ItemType m_type = ItemType::e_Property;
    const GroupTreeItem *m_parent = nullptr;
};

class GroupTreeItem : public TreeItem
{
public:
    GroupTreeItem(ItemType type, const GroupTreeItem *parent = nullptr)
        : TreeItem(type, parent)
    {}

private:
    std::vector<TreeItem *> m_items;

    // TreeItem interface
public:
    int index_of_item(const TreeItem *p) const override;
    TreeItem *item_at(int index) override;
    const TreeItem *item_at(int index) const override;
    bool append_item(TreeItem *item) override;
    bool insert_item_at_index(TreeItem *item, int index) override;
    bool remove_item(TreeItem *item) override;
    bool move_item(int from, int to) override;
    int item_count() const override;
    bool is_group() const override { return true; }
};

} // namespace alive
#endif // TREEMODEL_H
