#ifndef SHAPEITEM_H
#define SHAPEITEM_H

#include <core/model/property/object.h>
#include <core/serializer/serializer_common.h>
#include <memory>
#include <string>
#include <vector>

namespace alive::core {
class Serializer;
}

namespace alive::model {

enum class ShapeType {
    e_None,
    e_Ellipse,
    e_Fill,
    e_GradientFill,
    e_Group,
    e_GradientStroke,
    e_Merge,
    e_Rectangle,
    e_Repeater,
    e_Round,
    e_Shape,
    e_Star,
    e_Stroke,
    e_Trim,
    e_Transform,
    e_Last,
};

const std::string &shape_type_to_name(ShapeType type);

class ShapeItem : public Object
{
public:
    ShapeItem(ShapeType type, Object *parent, const ObjectDescriptor &descriptor);
    ShapeItem(Object *object, const ShapeItem &other);
    virtual ~ShapeItem();

    //    virtual void decode(QJsonObject& in_obj, QList<QString>& out_messages);
    bool hidden() const { return m_hidden; }
    ShapeType type() const { return m_type; }

    bool is_object_visible() const override { return !m_hidden; }
    void set_object_visible(bool visible) override
    {
        if (m_hidden == visible) {
            m_hidden = !visible;
            notify_observers();
        }
    }

    virtual ShapeItem *clone_shape_item(Object *object) const = 0;
    ShapeItem *clone(Object *object) const override;

private:
    std::string m_match_name;
    ShapeType m_type;
    bool m_hidden = false;

private:
    ADD_FRIEND_SERIALIZERS
};

class PathShapeItem : public ShapeItem
{
public:
    PathShapeItem(ShapeType type, Object *object, const ObjectDescriptor &descriptor);
    PathShapeItem(Object *object, const PathShapeItem &other);

    PathDirection path_direction() const { return m_direction; }
    void set_path_direction(PathDirection direction);

protected:
    PathDirection m_direction = PathDirection::e_Clockwise;

private:
    ADD_FRIEND_SERIALIZERS
};

using ShapeItemPtr = std::unique_ptr<ShapeItem>;
using ShapeItems = std::vector<ShapeItemPtr>;

} // namespace alive::model

#endif // SHAPEITEM_H
