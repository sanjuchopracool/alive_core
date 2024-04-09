#include "font_manager.h"
#include <core/font.h>
#include <core/resource_manager.h>
#include <core/skia_conversion/skia_to_inae.h>

#include <core/logging/logger.h>
#include <filesystem>
#include <include/core/SkFontMgr.h>
#include <include/core/SkTypeface.h>
#include <map>
#include <memory>
#include <string>

namespace inae::core {
const std::string k_unknown_string = "Unknown";
std::unique_ptr<FontManager> the_instance;

struct FontInfo
{
    sk_sp<SkTypeface> typeface;
    bool is_system;
};

struct FontManagerPrivateData
{
    FontManagerPrivateData()
    {   
        constexpr std::size_t initial_capacity = 500;
        fonts.reserve(initial_capacity);
    }
    std::map<std::string, uint32_t> family_name_to_family_id;
    std::vector<std::vector<FontInfo>> fonts;
    std::vector<std::string> family_name;
};

FontManager::FontManager()
    : m_d(std::make_unique<FontManagerPrivateData>())
{
}

FontManager::~FontManager() {}

void FontManager::init()
{
    if (!the_instance) {
        the_instance.reset(new FontManager());
    }
}

FontManager &FontManager::instance()
{
    return *the_instance.get();
}

void FontManager::load_fonts_from_path(const std::filesystem::path &font_dir)
{
    namespace fs = std::filesystem;
    if (!fs::is_directory(font_dir)) {
        INAE_CORE_ERROR("Font directory {} does not exist!", font_dir.string());
        return;
    }

    for (auto &entry : fs::recursive_directory_iterator(font_dir)) {
        const auto &path = entry.path();
        if (path.extension() == ".ttf") {
            INAE_CORE_TRACE("Loading fonts from file {}", path.string());
            auto typeface = ResourceManager::system_font_manager().makeFromFile(path.c_str());
            add_typeface(typeface);
        }
    }
}

std::size_t FontManager::families_count() const
{
    return m_d->family_name.size();
}

int FontManager::family_index(const std::string &name)
{
    auto it = m_d->family_name_to_family_id.find(name);
    if (it != m_d->family_name_to_family_id.end()) {
        return static_cast<int>(it->second);
    }
    return -1;
}

std::pair<int, int> FontManager::default_index() const
{
    return {0, 0};
}

std::string FontManager::family_name(size_t family_id) const
{
    if (family_id < m_d->family_name.size()) {
        return m_d->family_name[family_id];
    }
    return k_unknown_string;
}

std::size_t FontManager::style_count(size_t family_id) const
{
    if (family_id < m_d->fonts.size()) {
        return m_d->fonts[family_id].size();
    }
    return 0;
}

std::string FontManager::style_name(size_t family_id, size_t font_id) const
{
    if (family_id < m_d->fonts.size() && font_id < m_d->fonts[family_id].size()) {
        return skia::to_string(m_d->fonts[family_id][font_id].typeface->fontStyle());
    }
    return k_unknown_string;
}

FontImpl *FontManager::font_impl(size_t family_id, size_t font_id) const
{
    if (family_id < m_d->fonts.size() && font_id < m_d->fonts[family_id].size()) {
        return m_d->fonts[family_id][font_id].typeface.get();
    }
    return nullptr;
}

void FontManager::add_typeface(sk_sp<SkTypeface> typeface, bool is_system)
{
    if (typeface) {
        SkString family;
        typeface->getFamilyName(&family);
        std::string family_name(family.c_str());
        INAE_CORE_TRACE("Adding Font Family {}, Style {}",
                        family_name,
                        skia::to_string(typeface->fontStyle()));
        uint32_t id = UINT32_MAX;
        auto &map = m_d->family_name_to_family_id;
        auto it = map.find(family_name);
        if (it == map.end()) {
            id = map.size();
            map[family_name] = id;
        } else {
            id = it->second;
        }

        auto &fonts = m_d->fonts;
        if (id >= fonts.size()) {
            fonts.emplace_back();
            fonts.back().emplace_back(typeface, is_system);
            m_d->family_name.emplace_back(family_name);
        } else {
            fonts[id].emplace_back(typeface, is_system);
        }
    }
}

} // namespace inae::core
