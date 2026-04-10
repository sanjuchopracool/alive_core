#include "shape_serializer.h"
#include "json_helper.h"
#include "serializer.h"
#include <core/model/shape_items/ellipse.h>
#include <core/model/shape_items/fill.h>
#include <core/model/shape_items/group.h>
#include <core/model/shape_items/polystar.h>
#include <core/model/shape_items/rectangle.h>
#include <core/model/shape_items/repeater.h>
#include <core/model/shape_items/shape.h>
#include <core/model/shape_items/shape_transformation.h>
#include <core/model/shape_items/stroke.h>
#include <core/model/shape_items/trim.h>
#include <map>
#include <string>

namespace alive::core {
using namespace model;

const char k_type_key[] = "ty";
const std::map<std::string, ShapeType> k_key_to_type{
    {"el", ShapeType::e_Ellipse},
    {"fl", ShapeType::e_Fill},
    {"gf", ShapeType::e_GradientFill},
    {"gr", ShapeType::e_Group},
    {"gs", ShapeType::e_GradientStroke},
    {"mm", ShapeType::e_Merge},
    {"rc", ShapeType::e_Rectangle},
    {"rp", ShapeType::e_Repeater},
    {"rd", ShapeType::e_Round},
    {"sh", ShapeType::e_Shape},
    {"sr", ShapeType::e_Star},
    {"st", ShapeType::e_Stroke},
    {"tm", ShapeType::e_Trim},
    {"tr", ShapeType::e_Transform},
};

ShapeType from_key(const std::string &key)
{
    auto found = k_key_to_type.find(key);
    if (found != k_key_to_type.end())
        return found->second;

    return ShapeType::e_None;
}

model::ShapeItem *ShapeSerializer::shape_from_object(json::JsonObject &in_object,
                                                     SerializationContext &out_messages,
                                                     const Version &version)
{
    ShapeItem *result = nullptr;
    auto type_json = json::json_pop(in_object, k_type_key);
    assert(!type_json.is_null());
    std::string type_str = type_json;
    ShapeType type = from_key(type_str);
    switch (type) {
    case ShapeType::e_Group: {
        auto group = new Group;
        core::Serializer::decode(*group, in_object, out_messages, version);
        result = group;
    } break;
    case ShapeType::e_Shape: {
        auto shape = new Shape;
        core::Serializer::decode(*shape, in_object, out_messages, version);
        result = shape;
    } break;
        //    case ShapeType::e_Merge:
        //        result = new Merge;
        //        break;
    case ShapeType::e_Fill: {
        Fill *fill = new Fill;
        core::Serializer::decode(*fill, in_object, out_messages, version);
        result = fill;
    } break;
    case ShapeType::e_Transform: {
        ShapeTransformation *transform = new ShapeTransformation;
        core::Serializer::decode(*transform, in_object, out_messages, version);
        result = transform;
    } break;
    case ShapeType::e_Ellipse: {
        Ellipse *ellipse = new Ellipse;
        core::Serializer::decode(*ellipse, in_object, out_messages, version);
        result = ellipse;
    } break;
    case ShapeType::e_Rectangle: {
        Rectangle *rectangle = new Rectangle;
        core::Serializer::decode(*rectangle, in_object, out_messages, version);
        result = rectangle;
    } break;
    case ShapeType::e_Star: {
        PolyStar *polystar = new PolyStar;
        core::Serializer::decode(*polystar, in_object, out_messages, version);
        result = polystar;

    } break;
    case ShapeType::e_Stroke: {
        Stroke *stroke = new Stroke;
        core::Serializer::decode(*stroke, in_object, out_messages, version);
        result = stroke;
    } break;
    case ShapeType::e_Trim: {
        Trim *trim = new Trim;
        core::Serializer::decode(*trim, in_object, out_messages, version);
        result = trim;
    } break;
    case ShapeType::e_Repeater: {
        Repeater *repeater = new Repeater;
        core::Serializer::decode(*repeater, in_object, out_messages, version);
        result = repeater;
    } break;
    default:
        std::string msg("Error: Unsupproted shape item type: ");
        out_messages.emplace_back(msg + type_str);
        break;
    }

    //        if (result) {
    //            alive result->decode(in_object, out_messages);

    //            const static QString msg("Error: Unsupproted %1 attribute %2");
    //            auto arg = type == ShapeType::e_None ? key : shape_type_names[static_cast<int>(type)];

    //            for (const auto &element : in_object.keys()) {
    //                out_messages.emplace_back(msg.arg(arg).arg(element));
    //            }
    //        }

    return result;
}

std::string ShapeSerializer::shape_type_to_string(model::ShapeType type)
{
    for (const auto &data : k_key_to_type) {
        if (data.second == type) {
            return data.first;
        }
    }
    return {};
}

void ShapeSerializer::to_json_object(const model::ShapeItems &shape_items,
                                     json::OrderedJsonObject &shapes_json)
{
    for (const auto &shape : shape_items) {
        switch (shape->type()) {
        case ShapeType::e_Rectangle: {
            const Rectangle *rectangle = static_cast<const Rectangle *>(shape.get());
            shapes_json.push_back(core::Serializer::to_json_object(*rectangle));
        } break;
        case ShapeType::e_Ellipse: {
            const Ellipse *ellipse = static_cast<const Ellipse *>(shape.get());
            shapes_json.push_back(core::Serializer::to_json_object(*ellipse));
        } break;
        case ShapeType::e_Star: {
            const PolyStar *polystart = static_cast<const PolyStar *>(shape.get());
            shapes_json.push_back(core::Serializer::to_json_object(*polystart));
        } break;
        case ShapeType::e_Fill: {
            const Fill *fill = static_cast<const Fill *>(shape.get());
            shapes_json.push_back(core::Serializer::to_json_object(*fill));
        } break;
        case ShapeType::e_Stroke: {
            const Stroke *stroke = static_cast<const Stroke *>(shape.get());
            shapes_json.push_back(core::Serializer::to_json_object(*stroke));
        } break;
        case ShapeType::e_Trim: {
            const Trim *trim = static_cast<const Trim *>(shape.get());
            shapes_json.push_back(core::Serializer::to_json_object(*trim));
        } break;
        case ShapeType::e_Group: {
            const Group *group = static_cast<const Group *>(shape.get());
            shapes_json.push_back(core::Serializer::to_json_object(*group));
        } break;
        case ShapeType::e_Shape: {
            const Shape *shape_item = static_cast<const Shape *>(shape.get());
            shapes_json.push_back(core::Serializer::to_json_object(*shape_item));
        } break;
        case ShapeType::e_Transform: {
            const ShapeTransformation *transform = static_cast<const ShapeTransformation *>(
                shape.get());
            shapes_json.push_back(core::Serializer::to_json_object(*transform));
        } break;
        case ShapeType::e_Repeater: {
            const Repeater *repeater = static_cast<const Repeater *>(shape.get());
            shapes_json.push_back(core::Serializer::to_json_object(*repeater));
        } break;
        //        case LayerType::e_Shape: {
        //            const EditorShapeLayer *ed_shape_layer = static_cast<const EditorShapeLayer *>(
        //                layer.get());
        //            ed_layer = ed_shape_layer;
        //            Serializer::save(static_cast<const ShapeLayer &>(*ed_shape_layer), object);
        //        } break;
        default:
            assert(false);
            break;
        }
    }
}

} // namespace alive::core
