#ifndef SHAPELAYER_H
#define SHAPELAYER_H

#include <core/model/layers/layer.h>
#include <core/model/shape_items/group.h>

namespace inae::model {

class ShapeLayer : public Layer
{
public:
    ShapeLayer(Object *object);
    ~ShapeLayer() override;

    void add_shape_item_at_index(ShapeItem *shape, int index);
    ShapeItem *remove_shape_item_at_index(int index);
    bool move_shapes(Object *group, int from, int to);
    const Group *group() const;
    const ShapeItems &shapes() const;
    int count() const;
    //    void decode(QJsonObject& in_obj,QList<QString> &out_messages) override;

public:
    std::unique_ptr<Group> m_group;

private:
    friend class inae::core::Serializer;
};
} // namespace inae::model
#endif // SHAPELAYER_H
