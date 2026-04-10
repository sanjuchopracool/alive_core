#include "lottie_parser.h"
#include <core/alive_version.h>
#include <core/logging/logger.h>
#include <core/model/composition.h>
#include <core/model/layers/image_layer.h>
#include <core/model/layers/null_layer.h>
#include <core/model/layers/precomposition_layer.h>
#include <core/model/layers/shape_layer.h>
#include <core/model/layers/solid_layer.h>
#include <core/model/layers/text_layer.h>
#include <core/private/json.h>
#include <core/serializer/editor_support.h>
#include <core/serializer/json_helper.h>
#include <core/serializer/serializer.h>

#include <fstream>

namespace {
const char k_precompse_uuid_key[] = "uuid";
} // namespace

namespace alive {
using namespace alive::core;

Corrade::Containers::Pointer<alive::model::Composition> LottieParser::parse_alive(
    const char *data, AliveProjectInformation &timeline_setting, Messages &out_messages)
{
    json::JsonObject in_obj = json::JsonObject::parse(data);
    if (in_obj.is_null())
        return nullptr;

    SerializationContext messages;
    auto comp = Serializer::composition_from_properties(in_obj, timeline_setting, messages);
    for (const auto &message : messages.msgs) {
        out_messages.emplace_back(message);
    }
    return comp;
}

OrderedJsonObject layer_to_json_object(const Layer *layer)
{
    OrderedJsonObject object;
    switch (layer->layer_type()) {
    case LayerType::e_Solid: {
        const SolidLayer *ed_solid_layer = static_cast<const SolidLayer *>(layer);
        Serializer::save(static_cast<const SolidLayer &>(*ed_solid_layer), object);
    } break;

    case LayerType::e_Shape: {
        const ShapeLayer *ed_shape_layer = static_cast<const ShapeLayer *>(layer);
        Serializer::save(static_cast<const ShapeLayer &>(*ed_shape_layer), object);
    } break;
    case LayerType::e_Text: {
        const TextLayer *text_layer = static_cast<const TextLayer *>(layer);
        Serializer::save(static_cast<const TextLayer &>(*text_layer), object);
    } break;
    case LayerType::e_Image: {
        const ImageLayer *text_layer = static_cast<const ImageLayer *>(layer);
        Serializer::save(static_cast<const ImageLayer &>(*text_layer), object);
    } break;
    case LayerType::e_Precomp: {
        const PrecompositionLayer *precomp_layer = static_cast<const PrecompositionLayer *>(layer);
        Serializer::save(static_cast<const PrecompositionLayer &>(*precomp_layer), object);
    } break;
    case LayerType::e_Null: {
        Serializer::save(*layer, object);
    } break;
    default:
        ALIVE_CORE_ERROR("Serialisation not supported for Layer Type {} !",
                        layer_type_name(layer->layer_type()));
        break;
    }
    return object;
}

std::string LottieParser::save_alive(const alive::model::Composition *comp,
                                    const AliveProjectInformation &info)
{
    OrderedJsonObject object = Serializer::composition_properties(comp, info);
    {
        OrderedJsonObject layers_json;
        for (const auto &layer : comp->layers()) {
            OrderedJsonObject object = layer_to_json_object(layer.get());
            layers_json.push_back(object);
        }
        Serializer::set_layers_json(object, std::move(layers_json));
    }
    return object.dump();
}

std::string LottieParser::save_alive_pre_comp(const alive::model::Composition *parent_comp,
                                             const AliveProjectInformation &info,
                                             const std::vector<model::Layer *> &layers,
                                             const std::string &uuid)
{
    OrderedJsonObject object = Serializer::composition_properties(parent_comp, info);
    object[k_precompse_uuid_key] = uuid;
    {
        OrderedJsonObject layers_json;
        for (const auto *layer : layers) {
            OrderedJsonObject object = layer_to_json_object(layer);
            layers_json.push_back(object);
        }
        Serializer::set_layers_json(object, std::move(layers_json));
    }
    return object.dump();
}

Corrade::Containers::Pointer<Precomposition> LottieParser::parse_alive_precomp(
    const char *data, AliveProjectInformation &info, Messages &out_messages)
{
    json::JsonObject obj = json::JsonObject::parse(data);
    if (obj.is_null())
        return nullptr;

    SerializationContext messages;
    auto comp = Serializer::precomposition_from_properties(obj, info, messages);
    for (const auto &message : messages.msgs) {
        out_messages.emplace_back(message);
    }
    return comp;
}

Corrade::Containers::Pointer<Precomposition> LottieParser::parse_alive_precomp(
    const std::string &file_path, Messages &out_messages)
{
    std::ifstream ifs(file_path.c_str());
    ifs.seekg(0, std::ios::end);
    size_t size = ifs.tellg();
    std::string buffer(size, ' ');
    ifs.seekg(0);
    ifs.read(&buffer[0], size);

    AliveProjectInformation info;
    return parse_alive_precomp(buffer.c_str(), info, out_messages);
}
} // namespace alive
