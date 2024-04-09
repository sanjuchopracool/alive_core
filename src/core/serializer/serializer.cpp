#include "serializer.h"
#include "json_property_parser.h"
#include <core/color_type.h>
#include <core/model/composition.h>
#include <core/model/layers/image_layer.h>
#include <core/model/layers/shape_layer.h>
#include <core/model/layers/solid_layer.h>
#include <core/model/layers/text_layer.h>
#include <core/model/layers/null_layer.h>
#include <core/model/shape_items/ellipse.h>
#include <core/model/shape_items/fill.h>
#include <core/model/shape_items/rectangle.h>
#include <core/model/shape_items/repeater.h>
#include <core/model/shape_items/shape.h>
#include <core/model/shape_items/shape_transformation.h>
#include <core/model/shape_items/stroke.h>
#include <core/model/shape_items/trim.h>
#include <core/model/transform.h>
#include <core/serializer/shape_serializer.h>

namespace inae::core {
using namespace inae::model;
using namespace inae::json;
namespace transform_keys {
const char k_anchor_key[] = "a";
const char k_position_key[] = "p";
const char k_scale_key[] = "s";
const char k_rotation_key[] = "r";
const char k_opacity_key[] = "o";
const char k_skew_key[] = "sk";
const char k_skew_axis_key[] = "sa";
} // namespace transform_keys

namespace rectangle_keys {
const char k_position_key[] = "p";
const char k_size_key[] = "s";
const char k_radius_key[] = "r";
} // namespace rectangle_keys

namespace shape_keys {
const char k_path_key[] = "ks";
}

namespace fill_keys {
const char k_opacity_key[] = "o";
const char k_color_key[] = "c";
const char k_fill_rule_key[] = "r";
const char k_fill_enabled_key[] = "fillEnabled";
} // namespace fill_keys

namespace stroke_keys {
const char k_opacity_key[] = "o";
const char k_color_key[] = "c";
const char k_width_key[] = "w";
//const QString line_cap_key = "lc";
//const QString line_join_key = "lj";
//const QString miter_limit_key = "ml";
//const QString dash_pattern_key = "d";
//const QString fill_enabled_key ="fillEnabled";
} // namespace stroke_keys

namespace trim_keys {
const char k_start_key[] = "s";
const char k_end_key[] = "e";
const char k_offset_key[] = "o";
const char k_trim_type_key[] = "m";
const char k_index_key[] = "ix";
} // namespace trim_keys
namespace layer_key {
const char k_name_key[] = "nm";
const char k_type_key[] = "ty";
const char k_index_key[] = "ind";
const char k_coordinate_space_key[] = "ddd";
const char k_in_frame_key[] = "ip";
const char k_out_frame_key[] = "op";
const char k_start_time_key[] = "st";
const char k_transform_key[] = "ks";
const char k_parent_key[] = "parent";
const char k_blend_mode_key[] = "bm";
const char k_masks_key[] = "masksProperties";
const char k_time_stretch_key[] = "sr";
const char k_matte_key[] = "tt";
const char k_hidden_key[] = "hd";
const char k_auto_orient_key[] = "ao";
const char k_is_used_as_matter[] = "td";
const char k_matte_parent[] = "tp";

const char k_inae_editor_color_index[] = "inae_eci";
const char k_inae_layer_locked[] = "inae_ll";
} // namespace layer_key

namespace solid_layer_key {
const char k_color_key[] = "sc";
const char k_width_key[] = "sw";
const char k_height_key[] = "sh";
} // namespace solid_layer_key

namespace shape_layer_key {
const char k_shapes_key[] = "shapes";
} // namespace shape_layer_key

namespace image_layer_key {
const char k_refid_key[] = "refId";
} // namespace image_layer_key

namespace text_layer_key {
const char k_document_data_key[] = "t";
const char k_text_data_key[] = "d";
} // namespace text_layer_key

namespace group_key {
const char k_shapes_key[] = "it";
} // namespace group_key

namespace composition_keys {
const char k_version_key[] = "v";
const char k_inae_version_key[] = "inae_v";
const char k_type_key[] = "ddd";
const char k_in_point_key[] = "ip";
const char k_out_point_key[] = "op";
const char k_framerate_key[] = "fr";
const char k_width_key[] = "w";
const char k_height_key[] = "h";
const char k_layers_key[] = "layers";
const char k_name_key[] = "nm";
const char k_uuid_key[] = "uuid";
//const char glyphs_key[] ="chars";
//const char fonts_key[] ="fonts";
const char k_asset_library_key[] = "assets";
//const char markers_key[] ="markers";
} // namespace composition_keys

namespace shape_item_keys {
const char k_match_name_key[] = "mn";
const char k_name_key[] = "nm";
const char k_hidden_key[] = "hd";
const char k_index_key[] = "ind";
const char k_type_key[] = "ty";
const char k_direction_key[] = "d";
} // namespace shape_item_keys

namespace repeater_key {
const char k_copies_key[] = "c";
const char k_offset_key[] = "o";
const char k_composite_key[] = "m";
const char k_transform_key[] = "tr";
} // namespace repeater_key

void Serializer::decode(Transform &transform,
                        JsonObject &in_obj,
                        SerializerWarnings &out_messages,
                        const Version &version)
{
    using namespace transform_keys;
    inae::json::lottie::decode(transform.m_anchor, json_pop(in_obj, k_anchor_key), out_messages);
    inae::json::lottie::decode(transform.m_position, json_pop(in_obj, k_position_key), out_messages);
    inae::json::lottie::decode(transform.m_scale, json_pop(in_obj, k_scale_key), out_messages);
    inae::json::lottie::decode(transform.m_opacity, json_pop(in_obj, k_opacity_key), out_messages);
    inae::json::lottie::decode(transform.m_rotation, json_pop(in_obj, k_rotation_key), out_messages);
}

OrderedJsonObject Serializer::to_json_object(const Transform &transform)
{
    using namespace transform_keys;
    OrderedJsonObject result;
    result[k_opacity_key] = inae::json::lottie::to_json_value(transform.m_opacity);
    result[k_rotation_key] = inae::json::lottie::to_json_value(transform.m_rotation);
    result[k_position_key] = inae::json::lottie::to_json_value(transform.m_position);
    result[k_anchor_key] = inae::json::lottie::to_json_value(transform.m_anchor);
    result[k_scale_key] = inae::json::lottie::to_json_value(transform.m_scale);
    return result;
}

void Serializer::decode(Rectangle &rect,
                        JsonObject &in_obj,
                        SerializerWarnings &out_messages,
                        const Version &version)
{
    using namespace rectangle_keys;
    inae::json::lottie::decode(rect.m_size, json_pop(in_obj, k_size_key), out_messages);
    inae::json::lottie::decode(rect.m_position, json_pop(in_obj, k_position_key), out_messages);
    inae::json::lottie::decode(rect.m_roundness, json_pop(in_obj, k_radius_key), out_messages);

    decode(static_cast<PathShapeItem &>(rect), in_obj, out_messages, version);
}

OrderedJsonObject Serializer::to_json_object(const Rectangle &rect)
{
    using namespace rectangle_keys;
    OrderedJsonObject result;
    result[k_size_key] = inae::json::lottie::to_json_value(rect.m_size);
    result[k_position_key] = inae::json::lottie::to_json_value(rect.m_position);
    result[k_radius_key] = inae::json::lottie::to_json_value(rect.m_roundness);
    to_json_object(static_cast<const PathShapeItem &>(rect), result);
    return result;
}

void Serializer::decode(Ellipse &ellipse,
                        JsonObject &in_obj,
                        SerializerWarnings &out_messages,
                        const Version &version)
{
    using namespace rectangle_keys;
    inae::json::lottie::decode(ellipse.m_size, json_pop(in_obj, k_size_key), out_messages);
    inae::json::lottie::decode(ellipse.m_position, json_pop(in_obj, k_position_key), out_messages);
    decode(static_cast<PathShapeItem &>(ellipse), in_obj, out_messages, version);
}

OrderedJsonObject Serializer::to_json_object(const Ellipse &ellipse)
{
    using namespace rectangle_keys;
    OrderedJsonObject result;
    result[k_size_key] = inae::json::lottie::to_json_value(ellipse.m_size);
    result[k_position_key] = inae::json::lottie::to_json_value(ellipse.m_position);
    to_json_object(static_cast<const PathShapeItem &>(ellipse), result);
    return result;
}

void Serializer::decode(Fill &fill,
                        JsonObject &in_obj,
                        SerializerWarnings &out_messages,
                        const Version &version)
{
    using namespace fill_keys;
    inae::json::lottie::decode(fill.m_opacity, json_pop(in_obj, k_opacity_key), out_messages);
    inae::json::lottie::decode(fill.m_color, json_pop(in_obj, k_color_key), out_messages);
    decode(static_cast<ShapeItem &>(fill), in_obj, out_messages, version);
}

OrderedJsonObject Serializer::to_json_object(const Fill &fill)
{
    using namespace fill_keys;
    OrderedJsonObject result;
    result[k_opacity_key] = inae::json::lottie::to_json_value(fill.m_opacity);
    result[k_color_key] = inae::json::lottie::to_json_value(fill.m_color);
    to_json_object(static_cast<const ShapeItem &>(fill), result);
    return result;
}

void Serializer::decode(Stroke &stroke,
                        JsonObject &in_obj,
                        SerializerWarnings &out_messages,
                        const Version &version)
{
    using namespace stroke_keys;
    inae::json::lottie::decode(stroke.m_opacity, json_pop(in_obj, k_opacity_key), out_messages);
    inae::json::lottie::decode(stroke.m_color, json_pop(in_obj, k_color_key), out_messages);
    inae::json::lottie::decode(stroke.m_width, json_pop(in_obj, k_width_key), out_messages);
    decode(static_cast<ShapeItem &>(stroke), in_obj, out_messages, version);
}

OrderedJsonObject Serializer::to_json_object(const Stroke &stroke)
{
    using namespace stroke_keys;
    OrderedJsonObject result;
    result[k_opacity_key] = inae::json::lottie::to_json_value(stroke.m_opacity);
    result[k_color_key] = inae::json::lottie::to_json_value(stroke.m_color);
    result[k_width_key] = inae::json::lottie::to_json_value(stroke.m_width);
    to_json_object(static_cast<const ShapeItem &>(stroke), result);
    return result;
}

void Serializer::decode(Repeater &repeater,
                        JsonObject &in_obj,
                        SerializerWarnings &out_messages,
                        const Version &version)
{
    using namespace repeater_key;
    inae::json::lottie::decode(repeater.m_copies, json_pop(in_obj, k_copies_key), out_messages);
    inae::json::lottie::decode(repeater.m_offset, json_pop(in_obj, k_offset_key), out_messages);
    auto tr = json_pop(in_obj, k_transform_key);
    decode(repeater.m_transformation, tr, out_messages, version);
    decode(static_cast<ShapeItem &>(repeater), in_obj, out_messages, version);
}

OrderedJsonObject Serializer::to_json_object(const Repeater &repeater)
{
    using namespace repeater_key;
    OrderedJsonObject result;
    result[k_copies_key] = inae::json::lottie::to_json_value(repeater.m_copies);
    result[k_offset_key] = inae::json::lottie::to_json_value(repeater.m_offset);
    result[k_transform_key] = to_json_object(repeater.m_transformation);
    to_json_object(static_cast<const ShapeItem &>(repeater), result);
    return result;
}

void Serializer::decode(Trim &trim,
                        JsonObject &in_obj,
                        SerializerWarnings &out_messages,
                        const Version &version)
{
    using namespace trim_keys;
    inae::json::lottie::decode(trim.m_start, json_pop(in_obj, k_start_key), out_messages);
    inae::json::lottie::decode(trim.m_end, json_pop(in_obj, k_end_key), out_messages);
    inae::json::lottie::decode(trim.m_offset, json_pop(in_obj, k_offset_key), out_messages);
    decode(static_cast<ShapeItem &>(trim), in_obj, out_messages, version);
}

OrderedJsonObject Serializer::to_json_object(const Trim &trim)
{
    using namespace trim_keys;
    OrderedJsonObject result;
    result[k_start_key] = inae::json::lottie::to_json_value(trim.m_start);
    result[k_end_key] = inae::json::lottie::to_json_value(trim.m_end);
    result[k_offset_key] = inae::json::lottie::to_json_value(trim.m_offset);
    to_json_object(static_cast<const ShapeItem &>(trim), result);
    return result;
}

void Serializer::decode(Group &group,
                        JsonObject &in_obj,
                        SerializerWarnings &out_messages,
                        const Version &version)
{
    using namespace group_key;
    auto val = json_pop(in_obj, k_shapes_key);
    if (!val.is_null()) {
        int index = 0;
        for (auto shape_json : val) {
            auto shape_item = core::ShapeSerializer::shape_from_object(shape_json,
                                                                       out_messages,
                                                                       version);
            if (shape_item) {
                group.add_shape_item_at_index(shape_item, index++);
            }
        }
    }
    decode(static_cast<ShapeItem &>(group), in_obj, out_messages, version);
}

OrderedJsonObject Serializer::to_json_object(const Group &group)
{
    using namespace group_key;
    OrderedJsonObject result;
    OrderedJsonObject shapes_json;
    core::ShapeSerializer::to_json_object(group.shapes(), shapes_json);
    result[k_shapes_key] = shapes_json;
    to_json_object(static_cast<const ShapeItem &>(group), result);
    return result;
}

void Serializer::decode(Shape &shape,
                        JsonObject &in_obj,
                        SerializerWarnings &out_messages,
                        const Version &version)
{
    using namespace shape_keys;
    inae::json::lottie::decode(shape.m_path, json_pop(in_obj, k_path_key), out_messages);
    decode(static_cast<ShapeItem &>(shape), in_obj, out_messages, version);
}

OrderedJsonObject Serializer::to_json_object(const Shape &shape)
{
    OrderedJsonObject result;
    to_json_object(static_cast<const ShapeItem &>(shape), result);
    return result;
}

void Serializer::decode(ShapeTransformation &transform,
                        JsonObject &in_obj,
                        SerializerWarnings &out_messages,
                        const Version &version)
{
    using namespace transform_keys;
    inae::json::lottie::decode(transform.m_anchor, json_pop(in_obj, k_anchor_key), out_messages);
    inae::json::lottie::decode(transform.m_position, json_pop(in_obj, k_position_key), out_messages);
    inae::json::lottie::decode(transform.m_scale, json_pop(in_obj, k_scale_key), out_messages);
    inae::json::lottie::decode(transform.m_opacity, json_pop(in_obj, k_opacity_key), out_messages);
    inae::json::lottie::decode(transform.m_rotation, json_pop(in_obj, k_rotation_key), out_messages);
    inae::json::lottie::decode(transform.m_skew, json_pop(in_obj, k_skew_key), out_messages);
    inae::json::lottie::decode(transform.m_skew_axis,
                               json_pop(in_obj, k_skew_axis_key),
                               out_messages);
    decode(static_cast<ShapeItem &>(transform), in_obj, out_messages, version);
}

OrderedJsonObject Serializer::to_json_object(const ShapeTransformation &transform)
{
    using namespace transform_keys;
    OrderedJsonObject result;
    result[k_opacity_key] = inae::json::lottie::to_json_value(transform.m_opacity);
    result[k_rotation_key] = inae::json::lottie::to_json_value(transform.m_rotation);
    result[k_position_key] = inae::json::lottie::to_json_value(transform.m_position);
    result[k_anchor_key] = inae::json::lottie::to_json_value(transform.m_anchor);
    result[k_scale_key] = inae::json::lottie::to_json_value(transform.m_scale);
    result[k_skew_key] = inae::json::lottie::to_json_value(transform.m_skew);
    result[k_skew_axis_key] = inae::json::lottie::to_json_value(transform.m_skew_axis);

    to_json_object(static_cast<const ShapeItem &>(transform), result);
    return result;
}

void Serializer::decode(ShapeItem &shape_item,
                        JsonObject &in_obj,
                        SerializerWarnings &out_messages,
                        const Version &version)
{
    using namespace shape_item_keys;
    auto temp = json_pop(in_obj, k_name_key);
    if (!temp.is_null()) {
        shape_item.m_name = temp;
    }
    temp = json_pop(in_obj, k_match_name_key);
    if (!temp.is_null()) {
        shape_item.m_match_name = temp;
    }
    temp = json_pop(in_obj, k_hidden_key);
    if (!temp.is_null()) {
        shape_item.m_hidden = temp;
    }
}

void Serializer::to_json_object(const ShapeItem &shape_item, OrderedJsonObject &result)
{
    using namespace shape_item_keys;
    result[k_name_key] = shape_item.m_name;
    result[k_match_name_key] = shape_item.m_match_name;
    result[k_hidden_key] = shape_item.m_hidden;
    result[k_type_key] = ShapeSerializer::shape_type_to_string(shape_item.type());
}

void Serializer::decode(PathShapeItem &shape_item,
                        JsonObject &in_obj,
                        SerializerWarnings &out_messages,
                        const Version &version)
{
    using namespace shape_item_keys;
    auto direction_json = json_pop(in_obj, k_direction_key);
    if (!direction_json.is_null()) {
        PathDirection dir = direction_json.get<PathDirection>();
        shape_item.m_direction = dir;
    }
    decode(static_cast<ShapeItem &>(shape_item), in_obj, out_messages, version);
}

void Serializer::to_json_object(const PathShapeItem &shape_item, OrderedJsonObject &result)
{
    using namespace shape_item_keys;
    result[k_direction_key] = static_cast<int>(shape_item.m_direction);
    to_json_object(static_cast<const ShapeItem &>(shape_item), result);
}

void Serializer::set_layers_to_composition(Composition *comp,
                                           std::vector<std::unique_ptr<Layer>> &layers)
{
    comp->set_layers(layers);
}

void Serializer::decode(Layer &layer,
                        JsonObject &in_obj,
                        SerializerWarnings &out_messages,
                        const Version &version)
{
    using namespace layer_key;
    layer.m_name = json_pop(in_obj, k_name_key);
    layer.m_index = json_pop(in_obj, k_index_key);

    assert(CoordinateSpace::e_Type2D
                 == static_cast<CoordinateSpace>(json_pop(in_obj, k_coordinate_space_key)));
    layer.m_in_point = json_pop(in_obj, k_in_frame_key);
    layer.m_out_point = json_pop(in_obj, k_out_frame_key);
    layer.m_start_time = json_pop(in_obj, k_start_time_key);
    auto tr = json_pop(in_obj, k_transform_key);
    Serializer::decode(layer.m_transform, tr, out_messages, version);

    auto parent = json_pop(in_obj, k_parent_key);
    if (!parent.is_null()) {
        layer.m_parent_index = parent;
    }

    JsonObject blend_mode = json_pop(in_obj, k_blend_mode_key);
    if (!blend_mode.is_null())
        layer.m_blend_mode = blend_mode.get<BlendMode>();
    if (layer.m_blend_mode != BlendMode::e_Normal) {
        out_messages.emplace_back("Error: Layer blender mode are not fully supported yet!");
    }

    //masks
    auto stretch = json_pop(in_obj, k_time_stretch_key);
    if (!stretch.is_null())
        layer.m_time_stretch = stretch;

    auto matte = json_pop(in_obj, k_matte_key);
    if (!matte.is_null()) {
        int matte_int = matte;
        if (matte_int >= 0 && matte_int < static_cast<int>(MatteType::e_Last)) {
            layer.m_matte = static_cast<MatteType>(matte_int);
        }
    }

    auto matte_layer_index = json_pop(in_obj, k_matte_parent);
    if (!matte_layer_index.is_null()) {
        layer.set_matte_layer_index(matte_layer_index);
    }

    if (layer.m_matte > MatteType::e_AlphaInvert) {
        out_messages.emplace_back("Error: Layer matte type is not supported yet!");
    }
    auto hidden = json_pop(in_obj, k_hidden_key);
    if (!hidden.is_null())
        layer.m_hidden = hidden.get<bool>();

    if (!json_pop(in_obj, k_auto_orient_key).is_null()) {
        out_messages.emplace_back("Error: Layer auto orient are not supported yet!");
    }

    auto color_index = inae::json::json_pop(in_obj, k_inae_editor_color_index);
    if (!color_index.is_null()) {
        layer.set_editor_color_index(static_cast<int>(color_index));
    }
    auto locked = inae::json::json_pop(in_obj, k_inae_layer_locked);
    if (!locked.is_null()) {
        layer.set_lock(static_cast<bool>(locked));
    }

    //    for (const auto &element : in_obj.keys()) {
    //        const static QString msg("Error: Unsupproted element : %1 in layer");
    //        out_messages.emplace_back(msg.arg(element));
    //    }
}

void Serializer::save(const Layer &layer, OrderedJsonObject &object)
{
    using namespace layer_key;
    object[k_coordinate_space_key] = static_cast<int>(CoordinateSpace::e_Type2D);
    object[k_index_key] = layer.m_index;
    object[k_type_key] = static_cast<int>(layer.layer_type());
    object[k_name_key] = layer.m_name.c_str();
    object[k_time_stretch_key] = layer.m_time_stretch;
    object[k_auto_orient_key] = 0;
    object[k_in_frame_key] = layer.m_in_point;
    object[k_out_frame_key] = layer.m_out_point;
    object[k_start_time_key] = layer.m_start_time;
    object[k_parent_key] = layer.m_parent_index;
    object[k_blend_mode_key] = static_cast<int>(layer.m_blend_mode);
    object[k_matte_key] = static_cast<int>(layer.m_matte);
    object[k_matte_parent] = layer.m_matte_layer_index;
    object[k_hidden_key] = layer.m_hidden;
    object[k_transform_key] = Serializer::to_json_object(layer.m_transform);

    // editor specific
    object[k_inae_editor_color_index] = layer.editor_color_index();
    object[k_inae_layer_locked] = layer.is_locked();
}

void Serializer::decode(SolidLayer &layer,
                        JsonObject &in_obj,
                        SerializerWarnings &out_messages,
                        const Version &version)
{
    using namespace solid_layer_key;
    std::string color = json_pop(in_obj, k_color_key);
    layer.m_color = skcolor_from_rgb_string(color);
    layer.m_width = json_pop(in_obj, k_width_key);
    layer.m_height = json_pop(in_obj, k_height_key);
    decode(static_cast<Layer &>(layer), in_obj, out_messages, version);
}

void Serializer::save(const SolidLayer &layer, OrderedJsonObject &obj)
{
    using namespace solid_layer_key;
    obj[k_color_key] = skcolor_to_rgb_string(layer.m_color);
    obj[k_width_key] = layer.m_width;
    obj[k_height_key] = layer.m_height;
    save(static_cast<const Layer &>(layer), obj);
}

void Serializer::decode(ShapeLayer &layer,
                        JsonObject &in_obj,
                        SerializerWarnings &out_messages,
                        const Version &version)
{
    using namespace shape_layer_key;
    auto val = json_pop(in_obj, k_shapes_key);
    if (!val.is_null()) {
        int index = 0;
        for (auto shape_json : val) {
            auto shape_item = core::ShapeSerializer::shape_from_object(shape_json,
                                                                       out_messages,
                                                                       version);
            if (shape_item) {
                layer.add_shape_item_at_index(shape_item, index++);
            }
        }
    }
    decode(static_cast<Layer &>(layer), in_obj, out_messages, version);
}

void Serializer::save(const ShapeLayer &layer, OrderedJsonObject &obj)
{
    using namespace shape_layer_key;

    OrderedJsonObject shapes_json;
    core::ShapeSerializer::to_json_object(layer.shapes(), shapes_json);
    obj[k_shapes_key] = shapes_json;
    save(static_cast<const Layer &>(layer), obj);
}

void Serializer::decode(TextLayer &layer,
                        JsonObject &in_obj,
                        SerializerWarnings &out_messages,
                        const Version &version)
{
    using namespace text_layer_key;
    auto val = json_pop(in_obj, k_document_data_key);
    if (!val.is_null()) {
        inae::json::lottie::decode(layer.m_text, json_pop(val, k_text_data_key), out_messages);
    }
    decode(static_cast<Layer &>(layer), in_obj, out_messages, version);
}

void Serializer::save(const TextLayer &layer, OrderedJsonObject &obj)
{
    using namespace text_layer_key;
    OrderedJsonObject doc_json;
    doc_json[k_text_data_key] = inae::json::lottie::to_json_value(layer.m_text);
    obj[k_document_data_key] = doc_json;
    save(static_cast<const Layer &>(layer), obj);
}

void Serializer::decode(ImageLayer &layer,
                        JsonObject &in_obj,
                        SerializerWarnings &out_messages,
                        const Version &version)
{
    using namespace image_layer_key;
    auto val = json_pop(in_obj, k_refid_key);
    if (!val.is_null()) {
        const std::string path = val;
        layer.set_path(path);
    }
    decode(static_cast<Layer &>(layer), in_obj, out_messages, version);
}

void Serializer::save(const ImageLayer &layer, OrderedJsonObject &obj)
{
    using namespace image_layer_key;
    obj[k_refid_key] = layer.path();
    save(static_cast<const Layer &>(layer), obj);
}

Layer *Serializer::layer_from_json(Composition *obj,
                                   JsonObject &in_value,
                                   SerializerWarnings &out_messages,
                                   const Version &version)
{
    Layer *result = nullptr;
    int t = get_layer_type(in_value);
    LayerType type = static_cast<LayerType>(t);

    switch (type) {
    case LayerType::e_Shape:
    {
        ShapeLayer* shape = new ShapeLayer(obj);
        decode(*shape, in_value, out_messages, version);
        result = shape;
    }
    break;
    case LayerType::e_Solid: {
        auto solid_layer = new SolidLayer(obj);
        decode(*solid_layer, in_value, out_messages, version);
        result = solid_layer;
    } break;
    case LayerType::e_Text:
    {
        TextLayer* layer = new TextLayer(obj);
        decode(*layer, in_value, out_messages, version);
        result = layer;
    }
    break;
    case LayerType::e_Image:
    {
        ImageLayer* layer = new ImageLayer(obj);
        decode(*layer, in_value, out_messages, version);
        result = layer;
    }
    break;
    case LayerType::e_Null:
    {
        NullLayer* layer = new NullLayer(obj);
        decode(*layer, in_value, out_messages, version);
        result = layer;
    }
    break;
    default:
        const static std::string msg("Error: Unsupproted layer type:");
        out_messages.emplace_back(msg + layer_type_name(type));
        break;
    }
    return result;
}

OrderedJsonObject Serializer::composition_properties(const Composition *comp)
{
    using namespace composition_keys;
    OrderedJsonObject object;
    object[k_version_key] = comp->version().c_str();
    //    object[k_inae_version_key] = comp->inae_version().toString();
    object[k_framerate_key] = comp->framerate();
    object[k_in_point_key] = comp->in_point();
    object[k_out_point_key] = comp->out_point();
    object[k_width_key] = comp->width();
    object[k_height_key] = comp->height();
    object[k_name_key] = comp->name();
    object[k_uuid_key] = comp->get_uuid();
    object[k_type_key] = static_cast<int>(CoordinateSpace::e_Type2D);
    return object;
}

void Serializer::set_layers_json(OrderedJsonObject &comp_json, OrderedJsonObject &&layers_json)
{
    comp_json[composition_keys::k_layers_key] = layers_json;
}

std::unique_ptr<Composition> Serializer::composition_from_properties(
    JsonObject &in_obj, SerializerWarnings &out_messages)
{
    using namespace composition_keys;
    std::string version = json_pop(in_obj, k_version_key);
    std::string name = json_pop(in_obj, k_name_key);

    CoordinateSpace type = static_cast<CoordinateSpace>(json_pop(in_obj, k_type_key));
    assert(type == CoordinateSpace::e_Type2D);

    auto val = json_pop(in_obj, k_in_point_key);
    assert(!val.is_null());
    KeyFrameTime in_point = val;

    val = json_pop(in_obj, k_out_point_key);
    assert(!val.is_null());
    KeyFrameTime out_point = val;

    val = json_pop(in_obj, k_framerate_key);
    assert(!val.is_null());
    FrameTimeType framerate = val;

    val = json_pop(in_obj, k_width_key);
    assert(!val.is_null());
    int width = val;

    val = json_pop(in_obj, k_height_key);
    assert(!val.is_null());
    int height = val;

    auto comp = std::make_unique<Composition>(std::move(name),
                                              std::move(version),
                                              width,
                                              height,
                                              in_point,
                                              out_point,
                                              framerate);
    if (json_pop(in_obj, k_asset_library_key).size()) {
        out_messages.emplace_back("Error: assets not supported yet!");
    }

    val = json_pop(in_obj, k_uuid_key);
    if (!val.is_null()) {
        std::string uuid = val;
        comp->set_uuid(uuid);
    }

    return comp;
}

JsonObject Serializer::get_layers_json_from_comp_json(JsonObject &comp)
{
    using namespace composition_keys;
    return json_pop(comp, k_layers_key);
}

int Serializer::get_layer_type(JsonObject &layer)
{
    using namespace layer_key;
    int t = json_pop(layer, k_type_key);
    return t;
}

std::unique_ptr<Composition> Serializer::parse_core(JsonObject &in_obj,
                                                      SerializerWarnings &out_messages,
                                                      const Version &version)
{
    auto comp = composition_from_properties(in_obj, out_messages);
    Composition::Layers layers;
    auto layers_json = get_layers_json_from_comp_json(in_obj);
    assert(!layers_json.is_null());
    for (auto &layer_json : layers_json) {
        auto layer = Serializer::layer_from_json(comp.get(), layer_json, out_messages, version);
        if (layer)
            layers.emplace_back(layer);
    }
    comp->set_layers(layers);
    //    Q_ASSERT(layers.empty());

    //    for (const auto &element : in_obj.keys()) {
    //        const static QString msg("Error: Unsupproted element : %1 in composition");
    //        out_messages.emplace_back(msg.arg(element));
    //    }
    return comp;
}

OrderedJsonObject Serializer::save_lottie(const Composition *comp)
{
    using namespace composition_keys;
    OrderedJsonObject object = composition_properties(comp);
    {
        OrderedJsonObject layers_json;
        for (const auto &layer : comp->layers()) {
            if (layer->layer_type() == LayerType::e_Solid) {
                OrderedJsonObject object;
                Serializer::save(static_cast<const SolidLayer &>(*layer.get()), object);
                layers_json.push_back(object);
            } else {
                assert(false);
            }
        }
        object[k_layers_key] = layers_json;
    }
    return object;
}
} // namespace inae::core
