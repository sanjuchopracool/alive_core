#ifndef GROUP_H
#define GROUP_H

#include <core/model/property/object.h>
#include <core/model/shape_items/shape_item.h>

namespace inae::core {
class Serializer;
}

namespace inae::model {

const ObjectDescriptor &group_descriptor();

class Group : public ShapeItem
{
public:
    Group(const ObjectDescriptor &descriptor = group_descriptor(), Object *object = nullptr);
    ~Group() override;

    void add_shape_item_at_index(ShapeItem *shape, int index);
    ShapeItem *remove_shape_item_at_index(int index);
    bool move_shapes(int from, int to);
    int count() const { return static_cast<int>(m_shapes.size()); }
    const ShapeItems &shapes() const { return m_shapes; }
    //    virtual void decode(QJsonObject& in_obj, QList<QString> &out_messages) override;

public:
    ShapeItems m_shapes;

private:
    friend class inae::core::Serializer;
};
} // namespace inae::model
#endif // GROUP_H
