#ifndef JSON_PROPERTY_PARSER_H
#define JSON_PROPERTY_PARSER_H
#
#include "json_helper.h"
#include <core/inae_version.h>
#include <core/model/keyframes/keyframe.h>
#include <core/model/property/dynamic_property.h>

namespace inae::json::lottie {

using namespace inae::model;
using namespace inae::json;
using JsonObject = nlohmann::json;
using SerializerWarnings = std::vector<std::string>;

static const char k_key_start[] = "s";
static const char k_key_end[] = "e";
static const char k_key_in_tan[] = "i";
static const char k_key_out_tan[] = "o";
static const char k_key_sp_in[] = "ti";
static const char k_key_sp_out[] = "to";
static const char k_key_hold[] = "h";
static const char k_data_key[] = "k";
static const char k_time_key[] = "t";
static const char k_array_key[] = "a";
static const char k_inae_expr_key[] = "inae_ex";
static const char k_inae_expr_enabled_key[] = "inae_ex_enabled";

struct DecoderHelper
{
    template<typename T>
    static void do_decode(T &in_obj, JsonObject &in_value)
    {
        decode(in_obj, in_value);
    } // namespace inae::Lottie

    template<typename T>
    static JsonObject to_json_value(const T &in_obj)
    {
        return json_value(in_obj);
    }
};

template<typename T>
void decode(DynamicProperty<T> &property,
            JsonObject obj,
            SerializerWarnings &out_messages,
            bool is_array = false)
{
    JsonObject value = obj[k_data_key];
    int is_keyframed = 0;
    JsonObject keyframed_obj = json_pop(obj, k_array_key);
    if (!keyframed_obj.is_null())
        is_keyframed = keyframed_obj;

    if (is_array) {
        is_keyframed = true;
    }
    if (is_keyframed) {
        KeyFrames keyframes;
        for (auto &object : value) {
            T start_value = {};
            Vec2D in_tangent = {};
            Vec2D out_tangent = {};
            Vec3D sp_in_tan = {};
            Vec3D sp_out_tan = {};

            bool hold = false;

            FrameTimeType time = object[k_time_key];
            if (object.contains(k_key_start))
                DecoderHelper::do_decode<T>(start_value, object[k_key_start]);

            // TODO: check if k_key_end required anymore
            //            if (object.contains(k_key_end))
            //                DecoderHelper::do_decode<T>(prev_end_value, object[k_key_end]);

            if (object.contains(k_key_in_tan))
                decode_keyframe_tangent(in_tangent, object[k_key_in_tan]);

            if (object.contains(k_key_out_tan))
                decode_keyframe_tangent(out_tangent, object[k_key_out_tan]);

            if (object.contains(k_key_hold))
                hold = object[k_key_hold];

            if (object.contains(k_key_sp_in)) {
                json::decode(sp_in_tan, object[k_key_sp_in]);
                //                Q_ASSERT(is_null(sp_in_tan));
            }

            if (object.contains(k_key_sp_out)) {
                json::decode(sp_out_tan, object[k_key_sp_out]);
                //                Q_ASSERT(is_null(sp_out_tan));
            }

            KeyFrameTyped<T> *keyframe = new KeyFrameTyped<T>(&property,
                                                              start_value,
                                                              time,
                                                              hold,
                                                              in_tangent,
                                                              out_tangent,
                                                              sp_in_tan,
                                                              sp_out_tan);
            JsonObject temp = json_pop(object, k_inae_expr_key);
            if (!temp.is_null()) {
                keyframe->set_expression(temp.get<std::string>());
                keyframe->enable_expression(object[k_inae_expr_enabled_key].get<bool>());
            }
            keyframes.emplace_back(keyframe);
        }

        property.set_keyframes(keyframes);
    } else {
        if (!value.is_null()) {
            T value_object;
            DecoderHelper::do_decode<T>(value_object, value);
            property.set_value(value_object);
        }

        JsonObject expr_str_obj = json_pop(obj, k_inae_expr_key);
        if (!expr_str_obj.is_null()) {
            std::string expr = expr_str_obj;
            if (expr.size()) {
                property.set_expression(expr, 0);
            }
        }
    }
}

template<typename T>
JsonObject to_json_value(const DynamicProperty<T> &property, bool save_as_array = false)
{
    JsonObject result;
    if (property.value_type() == PropertyAnimationType::e_Constant) {
        if (save_as_array) {
            JsonObject array;
            JsonObject object;
            object[k_key_start] = DecoderHelper::to_json_value<T>(property.get_value());
            object[k_time_key] = 0;
            object[k_inae_expr_key] = property.get_expression(0);
            object[k_inae_expr_enabled_key] = true;
            array.push_back(object);
            result[k_data_key] = array;
            result[k_array_key] = 1;
        } else {
            result[k_data_key] = DecoderHelper::to_json_value<T>(property.get_value());
            result[k_inae_expr_key] = property.get_expression(0);
            result[k_array_key] = 0;
        }
    } else if (property.value_type() == PropertyAnimationType::e_KeyFramed) {
        JsonObject array;
        const KeyFrames &keyframes = property.get_keyframes();
        for (const KeyFrame *keyframe_base : keyframes) {
            const KeyFrameTyped<T> *keyframe = static_cast<const KeyFrameTyped<T> *>(keyframe_base);
            JsonObject object;

            if (!is_null(keyframe->in_tangent()))
                object[k_key_in_tan] = keyframe_tangent_to_json(keyframe->in_tangent());

            if (!is_null(keyframe->out_tangent()))
                object[k_key_out_tan] = keyframe_tangent_to_json(keyframe->out_tangent());

            object[k_time_key] = keyframe->time();
            object[k_key_start] = DecoderHelper::to_json_value<T>(keyframe->value());
            if (keyframe->is_hold()) {
                object[k_key_hold] = keyframe->is_hold();
            }

            if (!is_null(keyframe->spatial_in_tangent()))
                object[k_key_sp_out] = DecoderHelper::to_json_value(keyframe->spatial_in_tangent());

            if (!is_null(keyframe->spatial_out_tangent()))
                object[k_key_sp_in] = DecoderHelper::to_json_value(keyframe->spatial_out_tangent());

            object[k_inae_expr_key] = keyframe->expression();
            object[k_inae_expr_enabled_key] = keyframe->expression_enabled();
            array.push_back(object);
        }
        result[k_data_key] = array;
        result[k_array_key] = 1;
    } else {
        assert(false);
    }

    return result;
}
} // namespace inae::json::lottie

#endif // JSON_PROPERTY_PARSER_H
