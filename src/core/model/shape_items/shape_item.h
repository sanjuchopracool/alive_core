#ifndef SHAPEITEM_H
#define SHAPEITEM_H

#include <memory>
#include <core/model/property/object.h>
#include <string>
#include <vector>

namespace inae::core {
class Serializer;
}

namespace inae::model {

enum class ShapeType {
    None,
    Ellipse,
    Fill,
    GradientFill,
    Group,
    GradientStroke,
    Merge,
    Rectangle,
    Repeater,
    Round,
    Shape,
    Star,
    Stroke,
    Trim,
    Transform,
    Last,
};

const std::string &shape_type_to_name(ShapeType type);

class ShapeItem : public Object
{
public:
    ShapeItem(ShapeType type, Object *object, const ObjectDescriptor &descriptor);
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

    const std::string &object_name() const override;
    std::string name() const { return m_name; }

private:
    std::string m_name;
    std::string m_match_name;
    ShapeType m_type;
    bool m_hidden = false;

private:
    friend class inae::core::Serializer;
};

class PathShapeItem : public ShapeItem
{
public:
    PathShapeItem(ShapeType type, Object *object, const ObjectDescriptor &descriptor);

    PathDirection path_direction() const { return m_direction; }
    void set_path_direction(PathDirection direction);

protected:
    PathDirection m_direction = PathDirection::e_Clockwise;

private:
    friend class inae::core::Serializer;
};

using ShapeItemPtr = std::unique_ptr<ShapeItem>;
using ShapeItems = std::vector<ShapeItemPtr>;

} // namespace inae::model

#endif // SHAPEITEM_H
