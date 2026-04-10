#ifndef SHAPELAYER_H
#define SHAPELAYER_H

#include <core/model/layers/layer.h>
#include <core/model/shape_items/group.h>

namespace alive::model {

class ShapeLayer : public Layer
{
public:
    ShapeLayer(Object *object, int index = -1);
    ~ShapeLayer() override;

    void add_shape_item_at_index(ShapeItem *shape, int index);
    ShapeItem *remove_shape_item_at_index(int index);
    bool move_shapes(Object *group, int from, int to);
    const Group *group() const;
    const ShapeItems &shapes() const;
    int count() const;

    ShapeLayer *clone_layer(Object *obj, int index) const override;

    // Scripting support
    Rectangle *search_rect(const std::string &name);
    Fill *search_fill(const std::string &name);
    Stroke *search_stroke(const std::string &name);

private:
    ShapeLayer(Object *object, const ShapeLayer &other, int index);

public:
    std::unique_ptr<Group> m_group;

private:
    ADD_FRIEND_SERIALIZERS
};
} // namespace alive::model
#endif // SHAPELAYER_H
