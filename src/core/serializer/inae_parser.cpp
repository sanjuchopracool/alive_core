#include "inae_parser.h"
#include <core/inae_version.h>
#include <core/logging/logger.h>
#include <core/model/composition.h>
#include <core/model/layers/image_layer.h>
#include <core/model/layers/null_layer.h>
#include <core/model/layers/shape_layer.h>
#include <core/model/layers/solid_layer.h>
#include <core/model/layers/text_layer.h>
#include <core/private/json.h>
#include <core/serializer/json_helper.h>
#include <core/serializer/serializer.h>

using namespace inae::core;
namespace {
const char k_inae_version[] = "inae_version";
const char k_inae_range_start[] = "inae_range_start";
const char k_inae_range_end[] = "inae_range_end";
const char k_inae_play_range_start[] = "inae_play_range_start";
const char k_inae_play_range_end[] = "inae_play_range_end";
} // namespace

Layer *editor_layer_from_json(inae::model::Composition *obj,
                              JsonObject &in_value,
                              inae::core::SerializerWarnings &out_messages,
                              const inae::Version &version)
{
    Layer *result = nullptr;
    int t = Serializer::get_layer_type(in_value);
    LayerType type = static_cast<LayerType>(t);

    switch (type) {
    case LayerType::e_Shape: {
        auto shape_layer = new ShapeLayer(obj);
        inae::core::Serializer::decode(static_cast<ShapeLayer &>(*shape_layer),
                                       in_value,
                                       out_messages,
                                       version);
        result = shape_layer;
    } break;
    case LayerType::e_Solid: {
        auto solid_layer = new SolidLayer(obj);
        inae::core::Serializer::decode(static_cast<SolidLayer &>(*solid_layer),
                                       in_value,
                                       out_messages,
                                       version);
        result = solid_layer;
    } break;
    case LayerType::e_Text: {
        auto text_layer = new TextLayer(obj);
        inae::core::Serializer::decode(static_cast<TextLayer &>(*text_layer),
                                       in_value,
                                       out_messages,
                                       version);
        result = text_layer;
    } break;
    case LayerType::e_Image: {
        auto text_layer = new ImageLayer(obj);
        inae::core::Serializer::decode(static_cast<ImageLayer &>(*text_layer),
                                       in_value,
                                       out_messages,
                                       version);
        result = text_layer;
    } break;
    case LayerType::e_Null: {
        result = new NullLayer(obj);
        inae::core::Serializer::decode(*result, in_value, out_messages, version);
    } break;
    default:
        const static std::string msg("Error: Unsupproted layer type:");
        out_messages.emplace_back(msg + layer_type_name(type));
        break;
    }
    return result;
}

std::unique_ptr<inae::model::Composition> inae::core::parser::parse_inae(
    const char *data, InaeProjectInformation &timeline_setting, ParserMessages &out_messages)
{
    json::JsonObject obj = json::JsonObject::parse(data);
    if (obj.is_null())
        return nullptr;

    auto json = json::json_pop(obj, k_inae_version);
    Version version;
    if (!json.is_null()) {
        std::string version_str = json;
        version = Version::from_string(version_str);
    }
    bool apply_default_setting = true;
    json = json::json_pop(obj, k_inae_range_start);
    if (!json.is_null()) {
        timeline_setting.m_range_start = json;
        timeline_setting.m_range_end = json::json_pop(obj, k_inae_range_end);
        timeline_setting.m_play_range_start = json::json_pop(obj, k_inae_play_range_start);
        timeline_setting.m_play_range_end = json::json_pop(obj, k_inae_play_range_end);
        apply_default_setting = false;
    }

    SerializerWarnings messages;
    auto comp = Serializer::composition_from_properties(obj, messages);
    if (apply_default_setting) {
        timeline_setting.m_range_start = 0;
        timeline_setting.m_range_end = comp->out_point();
        timeline_setting.m_play_range_start = 0;
        timeline_setting.m_play_range_end = timeline_setting.m_range_end;
    }
    Composition::Layers layers;
    auto layers_json = Serializer::get_layers_json_from_comp_json(obj);
    //    assert(!layers_json.is_null());
    for (auto &layer_json : layers_json) {
        auto layer = editor_layer_from_json(comp.get(), layer_json, messages, version);
        if (layer)
            layers.emplace_back(layer);
    }

    Serializer::set_layers_to_composition(comp.get(), layers);
    for (const auto &message : messages) {
        out_messages.emplace_back(message);
    }
    return comp;
}

std::string parser::save_inae(const model::Composition *comp, const InaeProjectInformation &info)
{
    inae::Version version;
    OrderedJsonObject object = Serializer::composition_properties(comp);
    object[k_inae_version] = version.to_string();
    object[k_inae_range_start] = info.m_range_start;
    object[k_inae_range_end] = info.m_range_end;
    object[k_inae_play_range_start] = info.m_play_range_start;
    object[k_inae_play_range_end] = info.m_play_range_end;
    {
        OrderedJsonObject layers_json;
        for (const auto &layer : comp->layers()) {
            OrderedJsonObject object;
            switch (layer->layer_type()) {
            case LayerType::e_Solid: {
                const SolidLayer *ed_solid_layer = static_cast<const SolidLayer *>(layer.get());
                Serializer::save(static_cast<const SolidLayer &>(*ed_solid_layer), object);
            } break;

            case LayerType::e_Shape: {
                const ShapeLayer *ed_shape_layer = static_cast<const ShapeLayer *>(layer.get());
                Serializer::save(static_cast<const ShapeLayer &>(*ed_shape_layer), object);
            } break;
            case LayerType::e_Text: {
                const TextLayer *text_layer = static_cast<const TextLayer *>(layer.get());
                Serializer::save(static_cast<const TextLayer &>(*text_layer), object);
            } break;
            case LayerType::e_Image: {
                const ImageLayer *text_layer = static_cast<const ImageLayer *>(layer.get());
                Serializer::save(static_cast<const ImageLayer &>(*text_layer), object);
            } break;
            case LayerType::e_Null: {
                Serializer::save(*layer.get(), object);
            } break;
            default:
                INAE_CORE_ERROR("Serialisation not supported for Layer Type {} !",
                                layer_type_name(layer->layer_type()));
                break;
            }
            layers_json.push_back(object);
        }
        Serializer::set_layers_json(object, std::move(layers_json));
    }
    return object.dump();
}
