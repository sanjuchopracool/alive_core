#ifndef GROUP_H
#define GROUP_H

#include <core/model/property/object.h>
#include <core/model/shape_items/shape_item.h>

namespace alive::core {
class Serializer;
}

namespace alive::model {

class Fill;
class Stroke;
class Rectangle;
const ObjectDescriptor &group_descriptor();

class Group : public ShapeItem
{
public:
    Group(const ObjectDescriptor &descriptor = group_descriptor(), Object *object = nullptr);
    Group(Object *object, const Group &other);
    ~Group() override;

    void add_shape_item_at_index(ShapeItem *shape, int index);
    ShapeItem *remove_shape_item_at_index(int index);
    bool move_shapes(int from, int to);
    int count() const { return static_cast<int>(m_shapes.size()); }
    const ShapeItems &shapes() const { return m_shapes; }

    Group *clone_shape_item(Object *object) const override;

    // Scripting support
    Rectangle *search_rect(const std::string &name);
    Fill *search_fill(const std::string &name);
    Stroke *search_stroke(const std::string &name);

    template<typename T>
    T *search_type(ShapeType type, const std::string &name);

public:
    ShapeItems m_shapes;

private:
    ADD_FRIEND_SERIALIZERS
};
} // namespace alive::model
#endif // GROUP_H
