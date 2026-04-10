#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <Corrade/Containers/Pointer.h>
#include <core/alive_types/common_types.h>
#include <core/private/json.h>

namespace alive {
class Version;
class TagManager;

struct AliveProjectInformation;
} // namespace alive

namespace alive::model {
class Transform;
class Layer;
class SolidLayer;
class ShapeLayer;
class TextLayer;
class ImageLayer;
class PrecompositionLayer;
class Composition;
class Precomposition;
class ShapeItem;
class Rectangle;
class PolyStar;
class Ellipse;
class Fill;
class Stroke;
class Trim;
class PathShapeItem;
class Group;
class Shape;
class ShapeTransformation;
class Repeater;
class PropertyGroup;
} // namespace alive::model

namespace alive::core {
using namespace alive::model;
using namespace json;
struct SerializationContext
{
    std::vector<std::string> msgs;
    TagManager *tag_mgr = nullptr;
    void emplace_back(const std::string &str) { msgs.emplace_back(str); }
};

class Serializer
{
public:
    static Corrade::Containers::Pointer<Composition> parse_core(JsonObject &in_obj,
                                                                SerializationContext &out_messages,
                                                                const Version &version);
    static void decode(PolyStar &polystar,
                       JsonObject &in_obj,
                       SerializationContext &out_messages,
                       const Version &version);
    static OrderedJsonObject to_json_object(const PolyStar &polystart);

    static void decode(Rectangle &rect,
                       JsonObject &in_obj,
                       SerializationContext &out_messages,
                       const Version &version);
    static OrderedJsonObject to_json_object(const Rectangle &rect);

    static void decode(Ellipse &ellipse,
                       JsonObject &in_obj,
                       SerializationContext &out_messages,
                       const Version &version);
    static OrderedJsonObject to_json_object(const Ellipse &ellipse);

    static void decode(Fill &fill,
                       JsonObject &in_obj,
                       SerializationContext &out_messages,
                       const Version &version);
    static OrderedJsonObject to_json_object(const Fill &fill);

    static void decode(Stroke &stroke,
                       JsonObject &in_obj,
                       SerializationContext &out_messages,
                       const Version &version);
    static OrderedJsonObject to_json_object(const Stroke &stroke);

    static void decode(Repeater &repeater,
                       JsonObject &in_obj,
                       SerializationContext &out_messages,
                       const Version &version);
    static OrderedJsonObject to_json_object(const Repeater &repeater);

    static void decode(Trim &trim,
                       JsonObject &in_obj,
                       SerializationContext &out_messages,
                       const Version &version);
    static OrderedJsonObject to_json_object(const Trim &trim);

    static void decode(Group &group,
                       JsonObject &in_obj,
                       SerializationContext &out_messages,
                       const Version &version);
    static OrderedJsonObject to_json_object(const Group &group);
    static void decode(Shape &shape,
                       JsonObject &in_obj,
                       SerializationContext &out_messages,
                       const Version &version);
    static OrderedJsonObject to_json_object(const Shape &shape);

    static void decode(ShapeTransformation &transform,
                       JsonObject &in_obj,
                       SerializationContext &out_messages,
                       const Version &version);
    static OrderedJsonObject to_json_object(const ShapeTransformation &transform);

    static void decode(Transform &transform,
                       JsonObject &in_obj,
                       SerializationContext &out_messages,
                       const Version &version);
    static OrderedJsonObject to_json_object(const Transform &transform);

    static void decode(PropertyGroup &group,
                       JsonObject &in_obj,
                       SerializationContext &out_messages,
                       const Version &version,
                       bool with_revert_support = false);
    static OrderedJsonObject to_json_object(const PropertyGroup &group, bool only_key_val = false);

    static void decode(TagManager &tag_mgr,
                       JsonObject &in_obj,
                       SerializationContext &out_messages,
                       const Version &version);
    static OrderedJsonObject to_json_object(const TagManager &tag_mgr);

    static void decode(ShapeItem &shape_item,
                       JsonObject &in_obj,
                       SerializationContext &out_messages,
                       const Version &version);
    static void to_json_object(const ShapeItem &shape_item, OrderedJsonObject &result);
    static void decode(PathShapeItem &shape_item,
                       JsonObject &in_obj,
                       SerializationContext &out_messages,
                       const Version &version);
    static void to_json_object(const PathShapeItem &shape_item, OrderedJsonObject &result);

    static void decode(SolidLayer &layer,
                       JsonObject &in_obj,
                       SerializationContext &out_messages,
                       const Version &version);
    static void save(const SolidLayer &layer, OrderedJsonObject &obj);

    static void decode(ShapeLayer &layer,
                       JsonObject &in_obj,
                       SerializationContext &out_messages,
                       const Version &version);
    static void save(const ShapeLayer &layer, OrderedJsonObject &obj);
    static void decode(TextLayer &layer,
                       JsonObject &in_obj,
                       SerializationContext &out_messages,
                       const Version &version);
    static void save(const TextLayer &layer, OrderedJsonObject &obj);
    static void decode(ImageLayer &layer,
                       JsonObject &in_obj,
                       SerializationContext &out_messages,
                       const Version &version);
    static void save(const ImageLayer &layer, OrderedJsonObject &obj);

    static void decode(PrecompositionLayer &layer,
                       JsonObject &in_obj,
                       SerializationContext &out_messages,
                       const Version &version);
    static void save(const PrecompositionLayer &layer, OrderedJsonObject &obj);

    static void decode(Layer &layer,
                       JsonObject &in_obj,
                       SerializationContext &out_messages,
                       const Version &version);
    static void save(const Layer &layer, OrderedJsonObject &obj);
    static Layer *layer_from_json(Composition *obj,
                                  JsonObject &in_value,
                                  SerializationContext &out_messages,
                                  const Version &version);

    static OrderedJsonObject composition_properties(const Precomposition *comp,
                                                    const AliveProjectInformation &info);
    static void set_layers_json(OrderedJsonObject &comp_json, OrderedJsonObject &&layers_json);
    static Corrade::Containers::Pointer<Composition> composition_from_properties(
        JsonObject &in_obj,
        AliveProjectInformation &timeline_setting,
        SerializationContext &out_messages);
    static Corrade::Containers::Pointer<Precomposition> precomposition_from_properties(
        JsonObject &in_obj,
        AliveProjectInformation &timeline_setting,
        SerializationContext &out_messages);
    static JsonObject get_layers_json_from_comp_json(JsonObject &comp);
    static int get_layer_type(JsonObject &layer);
    static void set_layers_to_composition(alive::model::Precomposition *comp,
                                          std::vector<Corrade::Containers::Pointer<Layer>> &layers);
};
} // namespace alive::core

#endif // SERIALIZER_H
