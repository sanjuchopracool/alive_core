#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <memory>
#include <core/private/json.h>

namespace inae {
class Version;
}

namespace inae::model {
class Transform;
class Layer;
class SolidLayer;
class ShapeLayer;
class TextLayer;
class ImageLayer;
class Composition;
class ShapeItem;
class Rectangle;
class Ellipse;
class Fill;
class Stroke;
class Trim;
class PathShapeItem;
class Group;
class Shape;
class ShapeTransformation;
class Repeater;
} // namespace inae::model

namespace inae::core {
using namespace inae::model;
using namespace json;
using SerializerWarnings = std::vector<std::string>;

class Serializer
{
public:
    static std::unique_ptr<Composition> parse_core(JsonObject &in_obj,
                                                     SerializerWarnings &out_messages,
                                                     const Version &version);
    static OrderedJsonObject save_lottie(const Composition *comp);

    static void decode(Rectangle &rect,
                       JsonObject &in_obj,
                       SerializerWarnings &out_messages,
                       const Version &version);
    static OrderedJsonObject to_json_object(const Rectangle &rect);

    static void decode(Ellipse &ellipse,
                       JsonObject &in_obj,
                       SerializerWarnings &out_messages,
                       const Version &version);
    static OrderedJsonObject to_json_object(const Ellipse &ellipse);

    static void decode(Fill &fill,
                       JsonObject &in_obj,
                       SerializerWarnings &out_messages,
                       const Version &version);
    static OrderedJsonObject to_json_object(const Fill &fill);

    static void decode(Stroke &stroke,
                       JsonObject &in_obj,
                       SerializerWarnings &out_messages,
                       const Version &version);
    static OrderedJsonObject to_json_object(const Stroke &stroke);

    static void decode(Repeater &repeater,
                       JsonObject &in_obj,
                       SerializerWarnings &out_messages,
                       const Version &version);
    static OrderedJsonObject to_json_object(const Repeater &repeater);

    static void decode(Trim &trim,
                       JsonObject &in_obj,
                       SerializerWarnings &out_messages,
                       const Version &version);
    static OrderedJsonObject to_json_object(const Trim &trim);

    static void decode(Group &group,
                       JsonObject &in_obj,
                       SerializerWarnings &out_messages,
                       const Version &version);
    static OrderedJsonObject to_json_object(const Group &group);
    static void decode(Shape &shape,
                       JsonObject &in_obj,
                       SerializerWarnings &out_messages,
                       const Version &version);
    static OrderedJsonObject to_json_object(const Shape &shape);

    static void decode(ShapeTransformation &transform,
                       JsonObject &in_obj,
                       SerializerWarnings &out_messages,
                       const Version &version);
    static OrderedJsonObject to_json_object(const ShapeTransformation &transform);

    static void decode(Transform &transform,
                       JsonObject &in_obj,
                       SerializerWarnings &out_messages,
                       const Version &version);
    static OrderedJsonObject to_json_object(const Transform &transform);

    static void decode(ShapeItem &shape_item,
                       JsonObject &in_obj,
                       SerializerWarnings &out_messages,
                       const Version &version);
    static void to_json_object(const ShapeItem &shape_item, OrderedJsonObject &result);
    static void decode(PathShapeItem &shape_item,
                       JsonObject &in_obj,
                       SerializerWarnings &out_messages,
                       const Version &version);
    static void to_json_object(const PathShapeItem &shape_item, OrderedJsonObject &result);

    static void decode(SolidLayer &layer,
                       JsonObject &in_obj,
                       SerializerWarnings &out_messages,
                       const Version &version);
    static void save(const SolidLayer &layer, OrderedJsonObject &obj);

    static void decode(ShapeLayer &layer,
                       JsonObject &in_obj,
                       SerializerWarnings &out_messages,
                       const Version &version);
    static void save(const ShapeLayer &layer, OrderedJsonObject &obj);
    static void decode(TextLayer &layer,
                       JsonObject &in_obj,
                       SerializerWarnings &out_messages,
                       const Version &version);
    static void save(const TextLayer &layer, OrderedJsonObject &obj);
    static void decode(ImageLayer &layer,
                       JsonObject &in_obj,
                       SerializerWarnings &out_messages,
                       const Version &version);
    static void save(const ImageLayer &layer, OrderedJsonObject &obj);

    static void decode(Layer &layer,
                       JsonObject &in_obj,
                       SerializerWarnings &out_messages,
                       const Version &version);
    static void save(const Layer &layer, OrderedJsonObject &obj);
    static Layer *layer_from_json(Composition *obj,
                                  JsonObject &in_value,
                                  SerializerWarnings &out_messages,
                                  const Version &version);

    static OrderedJsonObject composition_properties(const Composition *comp);
    static void set_layers_json(OrderedJsonObject &comp_json, OrderedJsonObject &&layers_json);
    static std::unique_ptr<Composition> composition_from_properties(
        JsonObject &in_obj, SerializerWarnings &out_messages);
    static JsonObject get_layers_json_from_comp_json(JsonObject &comp);
    static int get_layer_type(JsonObject &layer);
    static void set_layers_to_composition(Composition *comp,
                                          std::vector<std::unique_ptr<Layer>> &layers);
};
} // namespace inae::core

#endif // SERIALIZER_H
