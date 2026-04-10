#ifndef JSON_PROPERTY_PARSER_H
#define JSON_PROPERTY_PARSER_H

#include "json_helper.h"
#include <core/font.h>
#include <core/alive_version.h>
#include <core/logging/logger.h>
#include <core/model/keyframes/keyframe.h>
#include <core/model/property/dynamic_property.h>
#include <core/model/property/property_group.h>
#include <core/model/property/property_type.h>
#include <core/model/property/static_property.h>
#include <core/model/property/static_property_factory.h>
#include <core/serializer/serializer.h>
#include <core/tagging/tag.h>
#include <core/tagging/tag_manager.h>
namespace alive::json::lottie {

using namespace alive::model;
using namespace alive::json;
using JsonObject = nlohmann::json;

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
static const char k_alive_expr_key[] = "alive_ex";
static const char k_alive_expr_enabled_key[] = "alive_ex_enabled";
static const char k_alive_linked_key[] = "alive_linked";
static const char k_alive_prop_name[] = "alive_name";
static const char k_alive_prop_id[] = "alive_id";
static const char k_alive_prop_type[] = "alive_type";
static const char k_alive_tags[] = "tags";

struct DecoderHelper
{
    template<typename T>
    static void do_decode(T &in_obj, JsonObject &in_value)
    {
        decode(in_obj, in_value);
    } // namespace alive::Lottie

    template<typename T>
    static JsonObject to_json_value(const T &in_obj)
    {
        return json_value(in_obj);
    }
};

template<typename T>
void decode(DynamicProperty<T> &property,
            JsonObject obj,
            core::SerializationContext &out_messages,
            bool is_array = false)
{
    JsonObject value = obj[k_data_key];
    int is_keyframed = 0;

    if (property.linkable()) {
        JsonObject linked_obj = json_pop(obj, k_alive_linked_key);
        if (!linked_obj.is_null()) {
            property.set_linked(linked_obj.get<bool>());
        }
    }

    JsonObject keyframed_obj = json_pop(obj, k_array_key);
    if (!keyframed_obj.is_null())
        is_keyframed = keyframed_obj;

    if (is_array) {
        is_keyframed = true;
    }
    if (is_keyframed) {
        KeyFrames keyframes;
        T prev_end_value = {};
        Vec2D prev_in_tangent = {};
        for (auto &object : value) {
            T start_value = prev_end_value;
            Vec2D in_tangent = {};
            Vec2D out_tangent = {};
            Vec3D sp_in_tan = {};
            Vec3D sp_out_tan = {};

            bool hold = false;

            FrameTimeType time = object[k_time_key];
            if (object.contains(k_key_start))
                DecoderHelper::do_decode<T>(start_value, object[k_key_start]);

            if (object.contains(k_key_end))
                DecoderHelper::do_decode<T>(prev_end_value, object[k_key_end]);

            if (object.contains(k_key_in_tan)) {
                decode_keyframe_tangent(in_tangent, object[k_key_in_tan]);
            } else {
                // in_tangent = {};
            }

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
                                                              prev_in_tangent,
                                                              out_tangent,
                                                              sp_in_tan,
                                                              sp_out_tan);
            JsonObject temp = json_pop(object, k_alive_expr_key);
            if (!temp.is_null()) {
                keyframe->set_expression(temp.get<std::string>());
                keyframe->enable_expression(object[k_alive_expr_enabled_key].get<bool>());
            }
            keyframes.emplace_back(keyframe);

            // tags
            {
                auto json = json_pop(object, k_alive_tags);
                if (!json.is_null()) {
                    std::vector<int> tag_ids = json;
                    for (auto &tagid : tag_ids) {
                        Tag *tag = out_messages.tag_mgr->tag(tagid);
                        if (tag) {
                            keyframe->add_tag(tag);
                        }
                    }
                }
            }

            prev_in_tangent = in_tangent;
        }

        property.set_keyframes(keyframes);
    } else {
        if (!value.is_null()) {
            T value_object;
            DecoderHelper::do_decode<T>(value_object, value);
            property.set_value(value_object);
        }

        JsonObject expr_str_obj = json_pop(obj, k_alive_expr_key);
        if (!expr_str_obj.is_null()) {
            std::string expr = expr_str_obj;
            if (expr.size()) {
                property.set_expression(expr, 0);
            }
        }

        JsonObject expr_enabled_obj = json_pop(obj, k_alive_expr_enabled_key);
        if (!expr_enabled_obj.is_null()) {
            property.enable_expression(expr_enabled_obj.get<bool>());
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
            object[k_alive_expr_key] = property.get_expression(0);
            object[k_alive_expr_enabled_key] = true;
            array.push_back(object);
            result[k_data_key] = array;
            result[k_array_key] = 1;
        } else {
            result[k_data_key] = DecoderHelper::to_json_value<T>(property.get_value());
            result[k_alive_expr_key] = property.get_expression(0);
            result[k_alive_expr_enabled_key] = property.expression_enabled();
            result[k_array_key] = 0;
        }
    } else if (property.value_type() == PropertyAnimationType::e_KeyFramed) {
        JsonObject array;
        const KeyFrames &keyframes = property.get_keyframes();
        for (int i = 0; i < keyframes.size(); ++i) {
            const KeyFrame *keyframe_base = keyframes[i];
            const KeyFrame *next_keyframe_base = (i < keyframes.size() - 1) ? keyframes[i + 1]
                                                                            : nullptr;
            const KeyFrameTyped<T> *keyframe = static_cast<const KeyFrameTyped<T> *>(keyframe_base);
            JsonObject object;

            if (next_keyframe_base) {
                if (!is_null(next_keyframe_base->in_tangent()))
                    object[k_key_in_tan] = keyframe_tangent_to_json(
                        next_keyframe_base->in_tangent());
            }

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

            object[k_alive_expr_key] = keyframe->expression();
            object[k_alive_expr_enabled_key] = keyframe->expression_enabled();
            std::vector<int> tags;
            for (const auto *tag : *keyframe_base) {
                tags.push_back(tag->id());
            }
            object[k_alive_tags] = tags;

            array.push_back(object);
        }
        result[k_data_key] = array;
        result[k_array_key] = 1;
    } else {
        assert(false);
    }

    if (property.linkable()) {
        result[k_alive_linked_key] = property.is_linked();
    }

    return result;
}

template<typename T>
JsonObject get_json_value_for_property(const Property *p)
{
    if constexpr (std::is_same_v<T, std::string>) {
        return static_cast<const PropertyTemplate<T> *>(p)->get_value();
    } else if constexpr (std::is_fundamental_v<T>) {
        return static_cast<const PropertyTemplate<T> *>(p)->get_value();
    } else {
        return DecoderHelper::to_json_value(
            static_cast<const PropertyTemplate<T> *>(p)->get_value());
    }

    return {};
}

inline JsonObject to_json_value(const Property *property, bool only_with_key = false)
{
    JsonObject result;
    if (property) {
        assertm(property->type() >= PropertyType::e_StaticPropertyStart
                    && property->type() < PropertyType::e_StaticPropertyEnd,
                "Only Static Property Supported So Far!");
        result[k_alive_prop_type] = static_cast<int>(property->type());
        result[k_alive_prop_name] = property->name();
        result[k_alive_prop_id] = property->id();

        auto m_json_value = [property]() -> JsonObject {
            switch (property->type()) {
            case alive::model::PropertyType::e_StaticSingleLineText: {
                return get_json_value_for_property<std::string>(property);
            }
            case alive::model::PropertyType::e_StaticColor3V: {
                return get_json_value_for_property<Color>(property);
            }
            case alive::model::PropertyType::e_StaticColor4ub: {
                return get_json_value_for_property<Color4ub>(property);
            }
            case alive::model::PropertyType::e_StaticInt:
            case alive::model::PropertyType::e_Int: {
                return get_json_value_for_property<int>(property);
            }
            case alive::model::PropertyType::e_StaticFloat:
            case alive::model::PropertyType::e_StaticPositiveFloat:
            case alive::model::PropertyType::e_StaticSizeFactor: {
                return get_json_value_for_property<float>(property);
            }
            case alive::model::PropertyType::e_Font: {
                return get_json_value_for_property<core::Font>(property);
            }
            case alive::model::PropertyType::e_Bool: {
                return get_json_value_for_property<bool>(property);
            }
            case alive::model::PropertyType::e_Direction: {
                return get_json_value_for_property<int>(property);
            }
            default:
                ALIVE_CORE_ERROR("Unsupported Static Property to Json!");
                break;
            }
            return {};
        };
        result[k_data_key] = m_json_value();
    }

    return result;
}

template<typename T>
void set_value_for_property(Property *p, JsonObject &obj, bool update_reset)
{
    if constexpr (std::is_same_v<T, std::string> || std::is_fundamental_v<T>) {
        if (update_reset) {
            static_cast<StaticPropertyWithReset<T> *>(p)->set_value(obj.get<T>());
        } else {
            static_cast<PropertyTemplate<T> *>(p)->set_value(obj.get<T>());
        }
    } else {
        T val;
        DecoderHelper::do_decode<T>(val, obj);
        if (update_reset) {
            static_cast<StaticPropertyWithReset<T> *>(p)->update_with_actual_value(val);
        } else {
            static_cast<PropertyTemplate<T> *>(p)->set_value(val);
        }
    }
}
auto set_property_value_functor =
    [](model::Property *property, JsonObject &obj, bool update_reset = false) {
        switch (property->type()) {
        case alive::model::PropertyType::e_StaticSingleLineText: {
            return set_value_for_property<std::string>(property, obj, update_reset);
        }
        case alive::model::PropertyType::e_StaticColor3V: {
            return set_value_for_property<Color>(property, obj, update_reset);
        }
        case alive::model::PropertyType::e_StaticColor4ub: {
            return set_value_for_property<Color4ub>(property, obj, update_reset);
        }
        case alive::model::PropertyType::e_StaticInt:
        case alive::model::PropertyType::e_Int: {
            return set_value_for_property<int>(property, obj, update_reset);
        }
        case alive::model::PropertyType::e_StaticFloat:
        case alive::model::PropertyType::e_StaticPositiveFloat:
        case alive::model::PropertyType::e_StaticSizeFactor: {
            return set_value_for_property<float>(property, obj, update_reset);
        }
        case alive::model::PropertyType::e_Font: {
            return set_value_for_property<core::Font>(property, obj, update_reset);
        }
        case alive::model::PropertyType::e_Bool: {
            return set_value_for_property<bool>(property, obj, update_reset);
        }
        case alive::model::PropertyType::e_Direction: {
            return set_value_for_property<int>(property, obj, update_reset);
        }
        default:
            ALIVE_CORE_ERROR("Unsupported Static Property from Json!");
            break;
        }
    };

void update_group_from_json(alive::model::PropertyGroup *grp,
                            JsonObject in_obj,
                            core::SerializationContext &out_messages)
{
    if (in_obj.is_array()) {
        std::set<int> indices;
        for (auto &obj : in_obj) {
            auto json = json_pop(obj, k_alive_prop_type);
            if (!json.is_null()) {
                PropertyType type = static_cast<PropertyType>(json.get<int>());
                assertm(type >= PropertyType::e_StaticPropertyStart
                            && type < PropertyType::e_StaticPropertyEnd,
                        "Only Static Property Supported So Far!");
                std::string name = json_pop(obj, k_alive_prop_name);
                std::string id = json_pop(obj, k_alive_prop_id);

                auto get_property = [&indices, grp](const std::string& id, int index) {
                    auto found = grp->search_with_id_from_index(id, index);
                    while (found.second != -1 && indices.contains(found.second)) {
                        found = grp->search_with_id_from_index(id, found.second);
                    }

                    if (found.second != -1) {
                        indices.emplace(found.second);
                    }
                    return found.first;
                };
                Property *p = get_property(id, -1);
                if (p) {
                    json = json_pop(obj, k_data_key);
                    set_property_value_functor(p, json, true);
                }
            }
        }
    }
}
Property *property_from_json(JsonObject obj,
                             core::SerializationContext &out_messages,
                             bool with_revert_support = false)
{
    auto json = json_pop(obj, k_alive_prop_type);
    if (!json.is_null()) {
        PropertyType type = static_cast<PropertyType>(json.get<int>());
        assertm(type >= PropertyType::e_StaticPropertyStart
                    && type < PropertyType::e_StaticPropertyEnd,
                "Only Static Property Supported So Far!");
        std::string name = json_pop(obj, k_alive_prop_name);
        std::string id = json_pop(obj, k_alive_prop_id);
        auto *property
            = alive::model::StaticPropertyFactory::create_static_property(type, with_revert_support);
        if (property) {
            if (property->name() != name) {
                property->set_name(name);
            }
            property->set_id(id);
            json = json_pop(obj, k_data_key);
            set_property_value_functor(property, json);
        }
        return property;
    }
    return nullptr;
}
} // namespace alive::json::lottie

#endif // JSON_PROPERTY_PARSER_H
