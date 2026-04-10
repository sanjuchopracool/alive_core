#include "alive_api.h"
#include <core/composition/composition_node.h>
#include <core/initializer.h>
#include <core/model/composition.h>
#include <core/serializer/editor_support.h>
#include <core/serializer/lottie_parser.h>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <map>
#include <stdio.h>
#include <string>

using namespace alive::core;
namespace {

struct CompositionData
{
    Corrade::Containers::Pointer<alive::model::Composition> comp;
    Corrade::Containers::Pointer<alive::model::RasterCompositionNode> comp_node;
    ;
};

static std::unique_ptr<Initializer> g_core_initializer;
static std::map<void *, CompositionData> g_opened_comps;

} // namespace

// Make sure the enums are in sync
// TODO: Check if there is any templated way to do it generically

static_assert(log_level::trace == static_cast<log_level>(spdlog::level::level_enum::trace));
static_assert(log_level::debug == static_cast<log_level>(spdlog::level::level_enum::debug));
static_assert(log_level::info == static_cast<log_level>(spdlog::level::level_enum::info));
static_assert(log_level::warn == static_cast<log_level>(spdlog::level::level_enum::warn));
static_assert(log_level::err == static_cast<log_level>(spdlog::level::level_enum::err));
static_assert(log_level::critical == static_cast<log_level>(spdlog::level::level_enum::critical));
static_assert(log_level::off == static_cast<log_level>(spdlog::level::level_enum::off));

constexpr char k_null__character = '\n';

void copy_to_error_str(char *error, std::string str)
{
    auto length = std::max<std::size_t>(1023, str.length());
    strncpy(error, str.c_str(), length);
    error[length] = k_null__character;
}
void zero_error_str(char *error)
{
    error[0] = k_null__character;
}
uint8_t alive_init(const char *resources_path, char *error, enum log_level level)
{
    uint8_t status = 1;
    zero_error_str(error);
    if (!g_core_initializer) {
        try {
            alive::core::Initializer::Params param(static_cast<spdlog::level::level_enum>(level),
                                                   resources_path);
            g_core_initializer = std::make_unique<Initializer>(param);
            status = 0;
        } catch (const std::filesystem::filesystem_error &e) {
            const std::string str(e.what());
            copy_to_error_str(error, str);
            status = 1;
        }
    } else {
        status = 1;
    }
    return status;
}

uint8_t alive_de_init(char *error)
{
    zero_error_str(error);
    if (g_core_initializer) {
        g_core_initializer.reset();
        g_opened_comps.clear();
        return 0;
    } else {
        return 1;
    }
}

void *alive_open_composition(const char *path, char *error)
{
    zero_error_str(error);
    using namespace std::filesystem;
    std::error_code err{};
    const std::filesystem::path path_item{path};
    if (!std::filesystem::exists(path_item, err)) {
        auto str = fmt::format("Composition file {} does not exist!", path);
        copy_to_error_str(error, str);
        return nullptr;
    }

    // TODO : full proof it
    std::ifstream ifs(path);
    ifs.seekg(0, std::ios::end);
    size_t size = ifs.tellg();
    std::string buffer(size, ' ');
    ifs.seekg(0);
    ifs.read(&buffer[0], size);

    // create a composition and its node here
    alive::AliveProjectInformation setting;
    alive::Messages messages;
    auto comp = alive::LottieParser::parse_alive(buffer.c_str(), setting, messages);
    if (!comp) {
        auto str = fmt::format("Could not create composition from file : {}!", path);
        copy_to_error_str(error, str);
        return nullptr;
    }
    auto comp_node = Corrade::Containers::pointer<alive::model::RasterCompositionNode>(comp.get());
    if (!comp) {
        auto str = fmt::format("Could not create composition node for : {}!", path);
        copy_to_error_str(error, str);
        return nullptr;
    }

    CompositionData data;
    data.comp.reset(comp.release());
    data.comp_node.reset(comp_node.release());

    auto node_ptr = data.comp_node.get();
    g_opened_comps.emplace(node_ptr, std::move(data));
    return node_ptr;
}

void alive_close_composition(void *comp, char *error)
{
    auto find = g_opened_comps.find(comp);
    if (find != g_opened_comps.end()) {
        g_opened_comps.erase(find);
    } else {
        auto str = fmt::format("Compsition does not exist!");
        copy_to_error_str(error, str);
    }
}

int32_t alive_compoistion_width(void *comp)
{
    return reinterpret_cast<alive::model::RasterCompositionNode *>(comp)->composition()->width();
}

int32_t alive_compoistion_height(void *comp)
{
    return reinterpret_cast<alive::model::RasterCompositionNode *>(comp)->composition()->height();
}

int32_t alive_composition_last_frame(void *comp)
{
    return reinterpret_cast<alive::model::RasterCompositionNode *>(comp)->composition()->out_point();
}

int32_t alive_composition_first_frame(void *comp)
{
    return reinterpret_cast<alive::model::RasterCompositionNode *>(comp)->composition()->in_point();
}

float alive_composition_fps(void *comp)
{
    return reinterpret_cast<alive::model::RasterCompositionNode *>(comp)->composition()->framerate();
}

const uint8_t *alive_composition_draw(
    void *comp, float time, int32_t width, int32_t height, alive_color color)
{
    auto comp_node = reinterpret_cast<alive::model::RasterCompositionNode *>(comp);
    comp_node->resize(width, height);
    alive::Color4ub bg_color(color.r, color.g, color.r, color.a);
    auto composition = const_cast<alive::model::Composition *>(comp_node->composition());
    composition->set_canvas_background_color(bg_color);
    comp_node->update(time, false);
    return comp_node->draw_data();
}
