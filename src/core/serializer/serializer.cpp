#include "serializer.h"
#include "json_property_parser.h"
#include <core/alive_types/color_type.h>
#include <core/model/composition.h>
#include <core/model/layers/image_layer.h>
#include <core/model/layers/null_layer.h>
#include <core/model/layers/precomposition_layer.h>
#include <core/model/layers/shape_layer.h>
#include <core/model/layers/solid_layer.h>
#include <core/model/layers/text_layer.h>
#include <core/model/property/property_group.h>
#include <core/model/shape_items/ellipse.h>
#include <core/model/shape_items/fill.h>
#include <core/model/shape_items/polystar.h>
#include <core/model/shape_items/rectangle.h>
#include <core/model/shape_items/repeater.h>
#include <core/model/shape_items/shape.h>
#include <core/model/shape_items/shape_transformation.h>
#include <core/model/shape_items/stroke.h>
#include <core/model/shape_items/trim.h>
#include <core/model/transform.h>
#include <core/serializer/editor_support.h>
#include <core/serializer/lottie_keys.h>
#include <core/serializer/shape_serializer.h>

namespace {

namespace {
const char k_alive_version[] = "alive_version";
const char k_alive_range_start[] = "alive_range_start";
const char k_alive_current_frame[] = "alive_current_frame";
const char k_alive_range_end[] = "alive_range_end";
const char k_alive_play_range_start[] = "alive_play_range_start";
const char k_alive_play_range_end[] = "alive_play_range_end";
const char k_alive_bg_color[] = "alive_bg_color";
const char k_alive_show_bg_color[] = "alive_show_bg_color";
const char k_alive_apply_script[] = "alive_apply_script";
const char k_alive_apply_script_on_change[] = "alive_apply_script_on_change";
const char k_alive_playground_script[] = "alive_playground_script";
const char k_alive_property_group[] = "alive_property_group";
const char k_alive_tags[] = "alive_tags";
} // namespace
} // namespace
namespace alive::core {
using namespace alive::model;
using namespace alive::json;

namespace {

Layer *editor_layer_from_json(alive::model::Precomposition *obj,
                              JsonObject &in_value,
                              alive::core::SerializationContext &out_messages,
                              const alive::Version &version)
{
    Layer *result = nullptr;
    int t = Serializer::get_layer_type(in_value);
    LayerType type = static_cast<LayerType>(t);

    switch (type) {
    case LayerType::e_Shape: {
        auto shape_layer = new ShapeLayer(obj);
        alive::core::Serializer::decode(static_cast<ShapeLayer &>(*shape_layer),
                                        in_value,
                                        out_messages,
                                        version);
        result = shape_layer;
    } break;
    case LayerType::e_Solid: {
        auto solid_layer = new SolidLayer(obj);
        alive::core::Serializer::decode(static_cast<SolidLayer &>(*solid_layer),
                                        in_value,
                                        out_messages,
                                        version);
        result = solid_layer;
    } break;
    case LayerType::e_Text: {
        auto text_layer = new TextLayer(obj);
        alive::core::Serializer::decode(static_cast<TextLayer &>(*text_layer),
                                        in_value,
                                        out_messages,
                                        version);
        result = text_layer;
    } break;
    case LayerType::e_Image: {
        auto text_layer = new ImageLayer(obj);
        alive::core::Serializer::decode(static_cast<ImageLayer &>(*text_layer),
                                        in_value,
                                        out_messages,
                                        version);
        result = text_layer;
    } break;
    case LayerType::e_Precomp: {
        auto precomp_layer = new PrecompositionLayer(obj);
        alive::core::Serializer::decode(static_cast<PrecompositionLayer &>(*precomp_layer),
                                        in_value,
                                        out_messages,
                                        version);
        result = precomp_layer;
    } break;
    case LayerType::e_Null: {
        result = new NullLayer(obj);
        alive::core::Serializer::decode(*result, in_value, out_messages, version);
    } break;
    default:
        const static std::string msg("Error: Unsupproted layer type:");
        out_messages.emplace_back(msg + layer_type_name(type));
        break;
    }
    return result;
}

template<typename CompType>
Corrade::Containers::Pointer<CompType> composition_of_type_from_properties(
    JsonObject &in_obj, AliveProjectInformation &timeline_setting, SerializationContext &out_messages)
{
    using namespace composition_keys;

    auto json = json::json_pop(in_obj, k_alive_version);
    Version alive_version;
    if (!json.is_null()) {
        std::string version_str = json;
        alive_version = Version::from_string(version_str);
    }

    alive::Version v_0_0_2(0, 0, 2);
    bool apply_default_setting = true;
    json = json::json_pop(in_obj, k_alive_range_start);
    if (!json.is_null()) {
        timeline_setting.m_range_start = json;
        timeline_setting.m_range_end = json::json_pop(in_obj, k_alive_range_end);
        timeline_setting.m_play_range_start = json::json_pop(in_obj, k_alive_play_range_start);
        timeline_setting.m_play_range_end = json::json_pop(in_obj, k_alive_play_range_end);

        if (alive_version >= v_0_0_2) {
            timeline_setting.m_current_frame = json::json_pop(in_obj, k_alive_current_frame);
        }
        json = json::json_pop(in_obj, k_alive_playground_script);
        if (!json.is_null()) {
            timeline_setting.m_playground_script = json;
        }
        apply_default_setting = false;
    }

    std::string version = json_pop(in_obj, k_version_key);
    std::string name = json_pop(in_obj, k_name_key);

    auto temp = json_pop(in_obj, k_type_key);
    CoordinateSpace type = temp.is_null() ? CoordinateSpace::e_Type2D
                                          : static_cast<CoordinateSpace>(temp);
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

    Corrade::Containers::Pointer<CompType> comp;
    if constexpr (std::is_same<alive::model::Composition, CompType>::value) { // is number
        comp = Corrade::Containers::pointer<CompType>(std::move(name),
                                                      std::move(version),
                                                      width,
                                                      height,
                                                      in_point,
                                                      out_point,
                                                      framerate);
    } else if constexpr (std::is_same<alive::model::Precomposition, CompType>::value) { // is number
        comp = Corrade::Containers::pointer<CompType>(nullptr,
                                                      std::move(name),
                                                      std::move(version),
                                                      width,
                                                      height,
                                                      in_point,
                                                      out_point,
                                                      framerate);
    } else {
        static_assert("Unknown Composition Type!!");
    }
    if (json_pop(in_obj, k_asset_library_key).size()) {
        out_messages.emplace_back("Error: assets not supported yet!");
    }

    out_messages.tag_mgr = comp->tag_manager();

    val = json_pop(in_obj, k_uuid_key);
    if (!val.is_null()) {
        std::string uuid = val;
        comp->set_uuid(uuid);
    }

    json = json::json_pop(in_obj, k_alive_bg_color);
    if (!json.is_null()) {
        comp->set_canvas_background_color(color_from_rgb_string(json));
    }

    if (alive_version >= v_0_0_2) {
        json = json::json_pop(in_obj, k_alive_show_bg_color);
        comp->set_show_canvas_bg_color(json.get<bool>());
    }

    if (alive_version >= Version(0, 0, 4)) {
        json = json::json_pop(in_obj, k_alive_apply_script);
        comp->set_apply_script(json.get<std::string>());

        json = json::json_pop(in_obj, k_alive_apply_script_on_change);
        comp->set_apply_script_on_change(json.get<bool>());
    }

    if (alive_version >= Version(0, 0, 5)) {
        json = json::json_pop(in_obj, k_alive_property_group);
        Serializer::decode(*comp->property_group(),
                           json,
                           out_messages,
                           alive_version,
                           std::is_same_v<alive::model::Precomposition, CompType>);
    }

    if (alive_version >= Version(0, 0, 6)) {
        json = json::json_pop(in_obj, k_alive_tags);
        Serializer::decode(*comp->tag_manager(), json, out_messages, alive_version);
    }

    if (apply_default_setting) {
        timeline_setting.m_range_start = 0;
        timeline_setting.m_range_end = comp->out_point();
        timeline_setting.m_play_range_start = 0;
        timeline_setting.m_play_range_end = timeline_setting.m_range_end;
    }

    Layers layers;
    auto layers_json = Serializer::get_layers_json_from_comp_json(in_obj);
    //    assert(!layers_json.is_null());
    for (auto &layer_json : layers_json) {
        auto layer = editor_layer_from_json(comp.get(), layer_json, out_messages, alive_version);
        if (layer)
            layers.emplace_back(layer);
    }

    Serializer::set_layers_to_composition(comp.get(), layers);
    return comp;
}
} // namespace
void Serializer::decode(Transform &transform,
                        JsonObject &in_obj,
                        SerializationContext &out_messages,
                        const Version &version)
{
    using namespace transform_keys;
    alive::json::lottie::decode(transform.m_anchor, json_pop(in_obj, k_anchor_key), out_messages);
    alive::json::lottie::decode(transform.m_position, json_pop(in_obj, k_position_key), out_messages);
    alive::json::lottie::decode(transform.m_scale, json_pop(in_obj, k_scale_key), out_messages);
    alive::json::lottie::decode(transform.m_opacity, json_pop(in_obj, k_opacity_key), out_messages);
    alive::json::lottie::decode(transform.m_rotation, json_pop(in_obj, k_rotation_key), out_messages);
    alive::json::lottie::decode(transform.m_skew, json_pop(in_obj, k_skew_key), out_messages);
    alive::json::lottie::decode(transform.m_skew_axis,
                               json_pop(in_obj, k_skew_axis_key),
                               out_messages);
}

OrderedJsonObject Serializer::to_json_object(const Transform &transform)
{
    using namespace transform_keys;
    OrderedJsonObject result;
    result[k_opacity_key] = alive::json::lottie::to_json_value(transform.m_opacity);
    result[k_rotation_key] = alive::json::lottie::to_json_value(transform.m_rotation);
    result[k_position_key] = alive::json::lottie::to_json_value(transform.m_position);
    result[k_anchor_key] = alive::json::lottie::to_json_value(transform.m_anchor);
    result[k_scale_key] = alive::json::lottie::to_json_value(transform.m_scale);
    result[k_skew_key] = alive::json::lottie::to_json_value(transform.m_skew);
    result[k_skew_axis_key] = alive::json::lottie::to_json_value(transform.m_skew_axis);
    return result;
}

void Serializer::decode(PropertyGroup &group,
                        JsonObject &in_obj,
                        SerializationContext &out_messages,
                        const Version &version,
                        bool with_revert_support)
{
    if (in_obj.is_array()) {
        for (auto &object : in_obj) {
            auto *p = alive::json::lottie::property_from_json(object,
                                                              out_messages,
                                                              with_revert_support);
            if (p) {
                group.add(p);
            }
        }
    }
}

OrderedJsonObject Serializer::to_json_object(const PropertyGroup &group, bool only_key_val)
{
    JsonObject result;
    auto count = group.count();
    for (size_t i = 0; i < count; ++i) {
        result.push_back(alive::json::lottie::to_json_value(group.at(i), only_key_val));
    }

    return result;
}

void Serializer::decode(TagManager &tag_mgr,
                        JsonObject &in_obj,
                        SerializationContext &out_messages,
                        const Version &version)
{
    for (auto &data : in_obj) {
        if (data.is_array() && data.size() == 2) {
            std::string name = data[0];
            Tag::TagId id = data[1];
            tag_mgr.create_tag(name, id);
        }
    }
}

OrderedJsonObject Serializer::to_json_object(const TagManager &tag_mgr)
{
    JsonObject result;
    for (const auto &data : tag_mgr) {
        JsonObject value;
        value.push_back(data->name());
        value.push_back(data->id());
        result.push_back(value);
    }
    return result;
}

void Serializer::decode(PolyStar &poly,
                        JsonObject &in_obj,
                        SerializationContext &out_messages,
                        const Version &version)
{
    using namespace polystar_keys;
    alive::json::lottie::decode(poly.m_points, json_pop(in_obj, k_num_points_key), out_messages);
    alive::json::lottie::decode(poly.m_position, json_pop(in_obj, k_position_key), out_messages);
    alive::json::lottie::decode(poly.m_rotation, json_pop(in_obj, k_rotation_key), out_messages);
    alive::json::lottie::decode(poly.m_inner_radius,
                               json_pop(in_obj, k_inner_radius_key),
                               out_messages);
    alive::json::lottie::decode(poly.m_outer_radius,
                               json_pop(in_obj, k_outer_radius_key),
                               out_messages);
    alive::json::lottie::decode(poly.m_inner_roundness,
                               json_pop(in_obj, k_inner_roundness_key),
                               out_messages);
    alive::json::lottie::decode(poly.m_outer_roundness,
                               json_pop(in_obj, k_outer_roundness_key),
                               out_messages);
    auto temp = json_pop(in_obj, k_star_type_key);
    poly.m_polystar_type = temp.is_null() ? PolyStar::Type::e_Star
                                          : static_cast<PolyStar::Type>(temp);

    decode(static_cast<PathShapeItem &>(poly), in_obj, out_messages, version);
}

OrderedJsonObject Serializer::to_json_object(const PolyStar &polystar)
{
    using namespace polystar_keys;
    OrderedJsonObject result;
    result[k_num_points_key] = alive::json::lottie::to_json_value(polystar.m_points);
    result[k_position_key] = alive::json::lottie::to_json_value(polystar.m_position);
    result[k_rotation_key] = alive::json::lottie::to_json_value(polystar.m_rotation);
    result[k_inner_radius_key] = alive::json::lottie::to_json_value(polystar.m_inner_radius);
    result[k_outer_radius_key] = alive::json::lottie::to_json_value(polystar.m_outer_radius);
    result[k_inner_roundness_key] = alive::json::lottie::to_json_value(polystar.m_inner_roundness);
    result[k_outer_roundness_key] = alive::json::lottie::to_json_value(polystar.m_outer_roundness);
    result[k_star_type_key] = static_cast<int>(polystar.m_polystar_type);

    to_json_object(static_cast<const PathShapeItem &>(polystar), result);
    return result;
}

void Serializer::decode(Rectangle &rect,
                        JsonObject &in_obj,
                        SerializationContext &out_messages,
                        const Version &version)
{
    using namespace rectangle_keys;
    alive::json::lottie::decode(rect.m_size, json_pop(in_obj, k_size_key), out_messages);
    alive::json::lottie::decode(rect.m_position, json_pop(in_obj, k_position_key), out_messages);
    alive::json::lottie::decode(rect.m_roundness, json_pop(in_obj, k_radius_key), out_messages);

    decode(static_cast<PathShapeItem &>(rect), in_obj, out_messages, version);
}

OrderedJsonObject Serializer::to_json_object(const Rectangle &rect)
{
    using namespace rectangle_keys;
    OrderedJsonObject result;
    result[k_size_key] = alive::json::lottie::to_json_value(rect.m_size);
    result[k_position_key] = alive::json::lottie::to_json_value(rect.m_position);
    result[k_radius_key] = alive::json::lottie::to_json_value(rect.m_roundness);
    to_json_object(static_cast<const PathShapeItem &>(rect), result);
    return result;
}

void Serializer::decode(Ellipse &ellipse,
                        JsonObject &in_obj,
                        SerializationContext &out_messages,
                        const Version &version)
{
    using namespace rectangle_keys;
    alive::json::lottie::decode(ellipse.m_size, json_pop(in_obj, k_size_key), out_messages);
    alive::json::lottie::decode(ellipse.m_position, json_pop(in_obj, k_position_key), out_messages);
    decode(static_cast<PathShapeItem &>(ellipse), in_obj, out_messages, version);
}

OrderedJsonObject Serializer::to_json_object(const Ellipse &ellipse)
{
    using namespace rectangle_keys;
    OrderedJsonObject result;
    result[k_size_key] = alive::json::lottie::to_json_value(ellipse.m_size);
    result[k_position_key] = alive::json::lottie::to_json_value(ellipse.m_position);
    to_json_object(static_cast<const PathShapeItem &>(ellipse), result);
    return result;
}

void Serializer::decode(Fill &fill,
                        JsonObject &in_obj,
                        SerializationContext &out_messages,
                        const Version &version)
{
    using namespace fill_keys;
    alive::json::lottie::decode(fill.m_opacity, json_pop(in_obj, k_opacity_key), out_messages);
    alive::json::lottie::decode(fill.m_color, json_pop(in_obj, k_color_key), out_messages);
    auto temp = json_pop(in_obj, k_fill_rule_key);
    fill.m_fill_rule = temp.is_null() ? FillRule::e_NonZeroWinding : static_cast<FillRule>(temp);
    decode(static_cast<ShapeItem &>(fill), in_obj, out_messages, version);
}

OrderedJsonObject Serializer::to_json_object(const Fill &fill)
{
    using namespace fill_keys;
    OrderedJsonObject result;
    result[k_opacity_key] = alive::json::lottie::to_json_value(fill.m_opacity);
    result[k_color_key] = alive::json::lottie::to_json_value(fill.m_color);
    result[k_fill_rule_key] = static_cast<int>(fill.m_fill_rule);
    to_json_object(static_cast<const ShapeItem &>(fill), result);
    return result;
}

void Serializer::decode(Stroke &stroke,
                        JsonObject &in_obj,
                        SerializationContext &out_messages,
                        const Version &version)
{
    using namespace stroke_keys;
    alive::json::lottie::decode(stroke.m_opacity, json_pop(in_obj, k_opacity_key), out_messages);
    alive::json::lottie::decode(stroke.m_color, json_pop(in_obj, k_color_key), out_messages);
    alive::json::lottie::decode(stroke.m_width, json_pop(in_obj, k_width_key), out_messages);
    decode(static_cast<ShapeItem &>(stroke), in_obj, out_messages, version);
}

OrderedJsonObject Serializer::to_json_object(const Stroke &stroke)
{
    using namespace stroke_keys;
    OrderedJsonObject result;
    result[k_opacity_key] = alive::json::lottie::to_json_value(stroke.m_opacity);
    result[k_color_key] = alive::json::lottie::to_json_value(stroke.m_color);
    result[k_width_key] = alive::json::lottie::to_json_value(stroke.m_width);
    to_json_object(static_cast<const ShapeItem &>(stroke), result);
    return result;
}

void Serializer::decode(Repeater &repeater,
                        JsonObject &in_obj,
                        SerializationContext &out_messages,
                        const Version &version)
{
    using namespace repeater_key;
    alive::json::lottie::decode(repeater.m_copies, json_pop(in_obj, k_copies_key), out_messages);
    alive::json::lottie::decode(repeater.m_offset, json_pop(in_obj, k_offset_key), out_messages);
    auto tr = json_pop(in_obj, k_transform_key);
    decode(repeater.m_transformation, tr, out_messages, version);
    decode(static_cast<ShapeItem &>(repeater), in_obj, out_messages, version);
}

OrderedJsonObject Serializer::to_json_object(const Repeater &repeater)
{
    using namespace repeater_key;
    OrderedJsonObject result;
    result[k_copies_key] = alive::json::lottie::to_json_value(repeater.m_copies);
    result[k_offset_key] = alive::json::lottie::to_json_value(repeater.m_offset);
    result[k_transform_key] = to_json_object(repeater.m_transformation);
    to_json_object(static_cast<const ShapeItem &>(repeater), result);
    return result;
}

void Serializer::decode(Trim &trim,
                        JsonObject &in_obj,
                        SerializationContext &out_messages,
                        const Version &version)
{
    using namespace trim_keys;
    alive::json::lottie::decode(trim.m_start, json_pop(in_obj, k_start_key), out_messages);
    alive::json::lottie::decode(trim.m_end, json_pop(in_obj, k_end_key), out_messages);
    alive::json::lottie::decode(trim.m_offset, json_pop(in_obj, k_offset_key), out_messages);
    decode(static_cast<ShapeItem &>(trim), in_obj, out_messages, version);
}

OrderedJsonObject Serializer::to_json_object(const Trim &trim)
{
    using namespace trim_keys;
    OrderedJsonObject result;
    result[k_start_key] = alive::json::lottie::to_json_value(trim.m_start);
    result[k_end_key] = alive::json::lottie::to_json_value(trim.m_end);
    result[k_offset_key] = alive::json::lottie::to_json_value(trim.m_offset);
    to_json_object(static_cast<const ShapeItem &>(trim), result);
    return result;
}

void Serializer::decode(Group &group,
                        JsonObject &in_obj,
                        SerializationContext &out_messages,
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
                        SerializationContext &out_messages,
                        const Version &version)
{
    using namespace shape_keys;
    alive::json::lottie::decode(shape.m_path, json_pop(in_obj, k_path_key), out_messages);
    decode(static_cast<ShapeItem &>(shape), in_obj, out_messages, version);
}

OrderedJsonObject Serializer::to_json_object(const Shape &shape)
{
    using namespace shape_keys;

    OrderedJsonObject result;
    result[k_path_key] = alive::json::lottie::to_json_value(shape.m_path);
    to_json_object(static_cast<const ShapeItem &>(shape), result);
    return result;
}

void Serializer::decode(ShapeTransformation &transform,
                        JsonObject &in_obj,
                        SerializationContext &out_messages,
                        const Version &version)
{
    using namespace transform_keys;
    alive::json::lottie::decode(transform.m_anchor, json_pop(in_obj, k_anchor_key), out_messages);
    alive::json::lottie::decode(transform.m_position, json_pop(in_obj, k_position_key), out_messages);
    alive::json::lottie::decode(transform.m_scale, json_pop(in_obj, k_scale_key), out_messages);
    alive::json::lottie::decode(transform.m_opacity, json_pop(in_obj, k_opacity_key), out_messages);
    alive::json::lottie::decode(transform.m_rotation, json_pop(in_obj, k_rotation_key), out_messages);
    alive::json::lottie::decode(transform.m_skew, json_pop(in_obj, k_skew_key), out_messages);
    alive::json::lottie::decode(transform.m_skew_axis,
                               json_pop(in_obj, k_skew_axis_key),
                               out_messages);
    decode(static_cast<ShapeItem &>(transform), in_obj, out_messages, version);
}

OrderedJsonObject Serializer::to_json_object(const ShapeTransformation &transform)
{
    using namespace transform_keys;
    OrderedJsonObject result;
    result[k_opacity_key] = alive::json::lottie::to_json_value(transform.m_opacity);
    result[k_rotation_key] = alive::json::lottie::to_json_value(transform.m_rotation);
    result[k_position_key] = alive::json::lottie::to_json_value(transform.m_position);
    result[k_anchor_key] = alive::json::lottie::to_json_value(transform.m_anchor);
    result[k_scale_key] = alive::json::lottie::to_json_value(transform.m_scale);
    result[k_skew_key] = alive::json::lottie::to_json_value(transform.m_skew);
    result[k_skew_axis_key] = alive::json::lottie::to_json_value(transform.m_skew_axis);

    to_json_object(static_cast<const ShapeItem &>(transform), result);
    return result;
}

void Serializer::decode(ShapeItem &shape_item,
                        JsonObject &in_obj,
                        SerializationContext &out_messages,
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
                        SerializationContext &out_messages,
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

void Serializer::set_layers_to_composition(Precomposition *comp,
                                           std::vector<Corrade::Containers::Pointer<Layer>> &layers)
{
    comp->set_layers(layers);
}

void Serializer::decode(Layer &layer,
                        JsonObject &in_obj,
                        SerializationContext &out_messages,
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

    auto color_index = alive::json::json_pop(in_obj, k_alive_editor_color_index);
    if (!color_index.is_null()) {
        layer.set_editor_color_index(static_cast<int>(color_index));
    }
    auto locked = alive::json::json_pop(in_obj, k_alive_layer_locked);
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
    object[k_alive_editor_color_index] = layer.editor_color_index();
    object[k_alive_layer_locked] = layer.is_locked();
}

void Serializer::decode(SolidLayer &layer,
                        JsonObject &in_obj,
                        SerializationContext &out_messages,
                        const Version &version)
{
    using namespace solid_layer_key;
    std::string color = json_pop(in_obj, k_color_key);
    layer.m_color = color_from_rgb_string(color);
    layer.m_width = json_pop(in_obj, k_width_key);
    layer.m_height = json_pop(in_obj, k_height_key);
    decode(static_cast<Layer &>(layer), in_obj, out_messages, version);
}

void Serializer::save(const SolidLayer &layer, OrderedJsonObject &obj)
{
    save(static_cast<const Layer &>(layer), obj);
    using namespace solid_layer_key;
    obj[k_color_key] = color_to_rgb_string(layer.m_color);
    obj[k_width_key] = layer.m_width;
    obj[k_height_key] = layer.m_height;
}

void Serializer::decode(ShapeLayer &layer,
                        JsonObject &in_obj,
                        SerializationContext &out_messages,
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
    save(static_cast<const Layer &>(layer), obj);
    using namespace shape_layer_key;    
    OrderedJsonObject shapes_json;
    core::ShapeSerializer::to_json_object(layer.shapes(), shapes_json);
    obj[k_shapes_key] = shapes_json;
}

void Serializer::decode(TextLayer &layer,
                        JsonObject &in_obj,
                        SerializationContext &out_messages,
                        const Version &version)
{
    using namespace text_layer_key;
    auto val = json_pop(in_obj, k_document_data_key);
    if (!val.is_null()) {
        alive::json::lottie::decode(layer.m_text, json_pop(val, k_text_data_key), out_messages);
    }
    decode(static_cast<Layer &>(layer), in_obj, out_messages, version);
}

void Serializer::save(const TextLayer &layer, OrderedJsonObject &obj)
{
    save(static_cast<const Layer &>(layer), obj);
    using namespace text_layer_key;
    OrderedJsonObject doc_json;
    doc_json[k_text_data_key] = alive::json::lottie::to_json_value(layer.m_text);
    obj[k_document_data_key] = doc_json;
}

void Serializer::decode(ImageLayer &layer,
                        JsonObject &in_obj,
                        SerializationContext &out_messages,
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

void Serializer::save(const PrecompositionLayer &layer, OrderedJsonObject &obj)
{
    save(static_cast<const Layer &>(layer), obj);
    using namespace precomp_layer_key;
    obj[k_refid_key] = layer.precomp_path();
    obj[k_alive_property_group] = to_json_object(*layer.property_group());
}

void Serializer::save(const ImageLayer &layer, OrderedJsonObject &obj)
{
    save(static_cast<const Layer &>(layer), obj);
    using namespace image_layer_key;
    obj[k_refid_key] = layer.path();
}

void Serializer::decode(PrecompositionLayer &layer,
                        JsonObject &in_obj,
                        SerializationContext &out_messages,
                        const Version &version)
{
    using namespace image_layer_key;
    auto val = json_pop(in_obj, k_refid_key);
    if (!val.is_null()) {
        const std::string path = val;
        layer.set_precomp_path(path);
    }
    decode(static_cast<Layer &>(layer), in_obj, out_messages, version);
    auto json = json_pop(in_obj, k_alive_property_group);
    if (!json.is_null()) {
        lottie::update_group_from_json(layer.property_group(), json, out_messages);
    }
}

Layer *Serializer::layer_from_json(Composition *obj,
                                   JsonObject &in_value,
                                   SerializationContext &out_messages,
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
    case LayerType::e_Precomp: {
        PrecompositionLayer *layer = new PrecompositionLayer(obj);
        decode(*layer, in_value, out_messages, version);
        result = layer;
    } break;
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

OrderedJsonObject Serializer::composition_properties(const Precomposition *comp,
                                                     const AliveProjectInformation &info)
{
    using namespace composition_keys;
    OrderedJsonObject object;
    object[k_version_key] = comp->version().c_str();
    //    object[k_alive_version_key] = comp->alive_version().toString();
    object[k_framerate_key] = comp->framerate();
    object[k_in_point_key] = comp->in_point();
    object[k_out_point_key] = comp->out_point();
    object[k_width_key] = comp->width();
    object[k_height_key] = comp->height();
    object[k_name_key] = comp->name();
    object[k_uuid_key] = comp->get_uuid();
    object[k_type_key] = static_cast<int>(CoordinateSpace::e_Type2D);

    // alive specific
    alive::Version version(0, 0, 6);
    object[k_alive_version] = version.to_string();
    object[k_alive_range_start] = info.m_range_start;
    object[k_alive_range_end] = info.m_range_end;
    object[k_alive_play_range_start] = info.m_play_range_start;
    object[k_alive_play_range_end] = info.m_play_range_end;
    object[k_alive_current_frame] = info.m_current_frame;
    object[k_alive_playground_script] = info.m_playground_script;
    object[k_alive_bg_color] = color_to_rgb_string(comp->canvas_background_color());
    object[k_alive_show_bg_color] = comp->show_background_color();
    object[k_alive_apply_script] = comp->apply_script();
    object[k_alive_apply_script_on_change] = comp->apply_script_on_change();
    object[k_alive_property_group] = to_json_object(*comp->m_prop_group);
    object[k_alive_tags] = to_json_object(*comp->tag_manager());

    return object;
}

void Serializer::set_layers_json(OrderedJsonObject &comp_json, OrderedJsonObject &&layers_json)
{
    comp_json[composition_keys::k_layers_key] = layers_json;
}

Corrade::Containers::Pointer<Composition> Serializer::composition_from_properties(
    JsonObject &in_obj, AliveProjectInformation &timeline_setting, SerializationContext &out_messages)
{
    return composition_of_type_from_properties<Composition>(in_obj, timeline_setting, out_messages);
}

Corrade::Containers::Pointer<Precomposition> Serializer::precomposition_from_properties(
    JsonObject &in_obj, AliveProjectInformation &timeline_setting, SerializationContext &out_messages)
{
    return composition_of_type_from_properties<Precomposition>(in_obj,
                                                               timeline_setting,
                                                               out_messages);
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

} // namespace alive::core
