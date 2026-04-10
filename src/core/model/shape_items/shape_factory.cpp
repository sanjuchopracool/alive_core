#include "shape_factory.h"
#include "shape_item.h"
#include <nlohmann/json.hpp>

#include <core/model/shape_items/ellipse.h>
#include <core/model/shape_items/fill.h>
#include <core/model/shape_items/group.h>
#include <core/model/shape_items/polystar.h>
#include <core/model/shape_items/rectangle.h>
#include <core/model/shape_items/repeater.h>
#include <core/model/shape_items/stroke.h>
#include <core/model/shape_items/trim.h>

#include <core/skia_conversion/skia_to_alive.h>

//#include <QJsonObject>
//#include <QMap>

namespace alive::model {
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

// ShapeItem *shape_from_object(json::JsonObject &in_object, SerializerWarnings &out_messages)
// {
//     ShapeItem *result = nullptr;
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

// return result;
// }

ShapeItem *ShapeFactory::create_shape_item(ShapeType shape_type)
{
    using namespace alive::core;
    ShapeItem *shape_item = nullptr;

    switch (shape_type) {
    case ShapeType::e_Rectangle: {
        Rectangle *rect = new Rectangle(nullptr);
        rect->set_size({100, 100});
        rect->set_position({0, 0});
        shape_item = rect;
    } break;
    case ShapeType::e_Ellipse: {
        Ellipse *ellipse = new Ellipse(nullptr);
        ellipse->set_size({100, 100});
        ellipse->set_position({0, 0});
        shape_item = ellipse;
    } break;
    case ShapeType::e_Star: {
        PolyStar *polystar = new PolyStar(nullptr);
        polystar->set_points(5);
        polystar->set_inner_radius(50);
        polystar->set_outer_radius(100);
        polystar->set_polystar_type(PolyStar::Type::e_Star);
        shape_item = polystar;
    } break;
    case ShapeType::e_Fill: {
        Fill *fill = new Fill(nullptr);
        fill->set_color(skia::skcolor_to_vec3d(SK_ColorMAGENTA));
        shape_item = fill;
    } break;
    case ShapeType::e_Stroke: {
        Stroke *stroke = new Stroke(nullptr);
        stroke->set_color(skia::skcolor_to_vec3d(SK_ColorBLACK));
        stroke->set_width(2);
        shape_item = stroke;
    } break;
    case ShapeType::e_Trim: {
        Trim *trim = new Trim(nullptr);
        shape_item = trim;
    } break;
    case ShapeType::e_Group: {
        Group *group = new Group;
        shape_item = group;
    } break;
    case ShapeType::e_Repeater: {
        Repeater *repeater = new Repeater;
        shape_item = repeater;
    } break;
    default:
        break;
    }
    return shape_item;
}

// std::vector<model::ShapeItem *> ShapeFactory::shapes_from_object(json::JsonObject &in_array,
//                                                                  SerializerWarnings &out_messages)
// {
//     std::vector<ShapeItem *> result;
//     for (auto shape : in_array) {
//         auto shape_item = shape_from_object(shape, out_messages);
//         if (shape_item) {
//             result.emplace_back(shape_item);
//         }
//     }
//     return result;
// }
} // namespace alive::model
