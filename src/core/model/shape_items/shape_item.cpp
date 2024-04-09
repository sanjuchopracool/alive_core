#include "shape_item.h"

namespace inae::model {

inline const std::vector<std::string> k_shape_type_names{
    "None",
    "Ellipse",
    "Fill",
    "GradientFill",
    "Group",
    "GradientStroke",
    "Merge",
    "Rectangle",
    "Repeater",
    "Round",
    "Shape",
    "Star",
    "Stroke",
    "Trim",
    "Transform",
};

ShapeItem::ShapeItem(ShapeType type, Object *object, const ObjectDescriptor &descriptor)
    : Object(object, descriptor)
    , m_type(type)
{}

ShapeItem::~ShapeItem()
{
    
}

const std::string &ShapeItem::object_name() const
{
    if (m_name.empty()) {
        return Object::object_name();
    }
    return m_name;
}

PathShapeItem::PathShapeItem(ShapeType type, Object *object, const ObjectDescriptor &descriptor)
    : ShapeItem(type, object, descriptor)
{}

void PathShapeItem::set_path_direction(PathDirection direction)
{
    if (m_direction != direction) {
        m_direction = direction;
        mark_dirty(true);
        notify_observers();
        mark_dirty(false);
    }
}

const std::string &shape_type_to_name(ShapeType type)
{
    return k_shape_type_names[static_cast<int>(type)];
}

//void ShapeItem::decode(QJsonObject &in_obj, QList<QString>& out_messages)
//{
//    m_name = in_obj.take(name_key).toString();
//    m_match_name = in_obj.take(match_name_key).toString();
//    m_hidden = in_obj.take(hidden_key).toBool();
//    auto index = in_obj.take(index_key);
//    if (!index.isUndefined()) {
//        static bool flag = true;
//        if(flag) {
//            flag = false;
//            out_messages.append("Warning: shape index is ignored!");
//        }
//    }
//}
} // namespace inae::model
