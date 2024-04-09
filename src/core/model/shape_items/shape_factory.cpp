#include "shape_factory.h"
#include "shape_item.h"
#include <nlohmann/json.hpp>

//#include "ellipse.h"
//#include "fill.h"
//#include "group.h"
//#include "merge.h"
//#include "polystar.h"
//#include "rectangle.h"
//#include "repeater.h"
//#include "shape.h"
//#include "shape_transformation.h"
//#include "stroke.h"
//#include "trim.h"

//#include <QJsonObject>
//#include <QMap>

namespace inae::model {
//const QString type_key = "ty";
//const QMap<QString, ShapeType> key_to_type{
//    {"el", ShapeType::Ellipse},
//    {"fl", ShapeType::Fill},
//    {"gf", ShapeType::GradientFill},
//    {"gr", ShapeType::Group},
//    {"gs", ShapeType::GradientStroke},
//    {"mm", ShapeType::Merge},
//    {"rc", ShapeType::Rectangle},
//    {"rp", ShapeType::Repeater},
//    {"rd", ShapeType::Round},
//    {"sh", ShapeType::Shape},
//    {"sr", ShapeType::Star},
//    {"st", ShapeType::Stroke},
//    {"tm", ShapeType::Trim},
//    {"tr", ShapeType::Transform},
//};

//ShapeType from_key(const QString& key)
//{
//    auto found = key_to_type.find(key);
//    if (found != key_to_type.end())
//        return  found.value();

//    return  ShapeType::None;
//}

ShapeItem *shape_from_object(json::JsonObject &in_object, SerializerWarnings &out_messages)
{
    ShapeItem *result = nullptr;
    //    auto val = in_object.take(type_key);
    //    Q_ASSERT(!val.isUndefined());
    //    auto key = val.toString();
    //    auto type = from_key(key);
    //    switch (type) {
    //    case ShapeType::Group:
    //        result = new Group;
    //        break;
    //    case ShapeType::Shape:
    //        result = new Shape;
    //        break;
    //    case ShapeType::Merge:
    //        result = new Merge;
    //        break;
    //    case ShapeType::Fill:
    //        result = new Fill;
    //        break;
    //    case ShapeType::Transform:
    //        result = new ShapeTransformation;
    //        break;
    //    case ShapeType::Ellipse:
    //        result = new Ellipse;
    //        break;
    //    case ShapeType::Rectangle:
    //        result = new Rectangle;
    //        break;
    //    case ShapeType::Star:
    //        result = new PolyStar;
    //        break;
    //    case ShapeType::Stroke:
    //        result = new Stroke;
    //        break;
    //    case ShapeType::Trim:
    //        result = new Trim;
    //        break;
    //    case ShapeType::Repeater:
    //        result = new Repeater;
    //        break;
    //    default:
    //        const static QString msg("Error: Unsupproted shape item type: %1");
    //        auto arg = type == ShapeType::None ? key : shape_type_names[static_cast<int>(type)];
    //        out_messages.emplace_back(msg.arg(arg));
    //        break;
    //    }

    //    if (result)
    //    {
    //        result->decode(in_object, out_messages);
    //        result->ShapeItem::decode(in_object, out_messages);

    //        const static QString msg("Error: Unsupproted %1 attribute %2");
    //        auto arg = type == ShapeType::None ? key : shape_type_names[static_cast<int>(type)];

    //        for(const auto& element : in_object.keys()) {
    //            out_messages.emplace_back(msg.arg(arg).arg(element));
    //        }
    //    }

    return  result;
}

std::vector<model::ShapeItem *> ShapeFactory::shapes_from_object(json::JsonObject &in_array,
                                                                 SerializerWarnings &out_messages)
{
    std::vector<ShapeItem *> result;
    for (auto shape : in_array) {
        auto shape_item = shape_from_object(shape, out_messages);
        if (shape_item) {
            result.emplace_back(shape_item);
        }
    }
    return result;
}
} // namespace inae::model
