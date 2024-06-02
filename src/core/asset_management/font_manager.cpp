#include "font_manager.h"
#include <core/font.h>
#include <core/resource_manager.h>
#include <core/skia_conversion/skia_to_inae.h>

#include "database.h"
#include <boost/algorithm/string.hpp>
#include <core/empty_values.h>
#include <core/logging/logger.h>
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
    DatabaseAssetId db_id;
    int style_name_index;
};

struct FamilyInfo
{
    std::string name;
    bool contains_system_fonts = false;
    bool contains_db_fonts = false;
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
    std::vector<FamilyInfo> family_info;
    int system_font_count = 0;
    bool system_fonts_loaded = false;

    // valid only in case of db fonts
    // keeping double memory for names, may be ok to release once all fonts loaded
    // TODO
    std::map<std::string, int32_t> style_name_to_style_id;
    std::vector<std::string> style_names;

    int32_t style_index(const std::string &name) const
    {
        auto it = style_name_to_style_id.find(name);
        if (it != style_name_to_style_id.end()) {
            return it->second;
        }
        return -1;
    }

    int32_t create_style_name_index(const std::string &name)
    {
        auto it = style_name_to_style_id.find(name);
        if (it == style_name_to_style_id.end()) {
            int32_t index = style_name_to_style_id.size();
            style_name_to_style_id.emplace(name, index);
            style_names.emplace_back(name);
            return index;
        }
        return it->second;
    }

    const std::string &style_name(int32_t index) const
    {
        if (index > 0 && index < style_names.size()) {
            return style_names[index];
        }
        return k_unknown_string;
    }
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

void FontManager::load_system_fonts()
{
    if (!m_d->system_fonts_loaded) {
        const auto &sys_font_mgr = inae::ResourceManager::system_font_manager();
        for (size_t i = 0; i < sys_font_mgr.countFamilies(); ++i) {
            const auto styleset = sys_font_mgr.createStyleSet(i);
            for (size_t j = 0; j < styleset->count(); ++j) {
                add_typeface(styleset->createTypeface(j), k_invalid_db_id);
            }
        }
        m_d->system_fonts_loaded = true;
    }
}

std::size_t FontManager::families_count() const
{
    return m_d->family_info.size();
}

int FontManager::family_index(const std::string &name) const
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

const std::string &FontManager::family_name(size_t family_id) const
{
    if (family_id < m_d->family_info.size()) {
        return m_d->family_info[family_id].name;
    }
    return k_unknown_string;
}

bool FontManager::family_contains_system_fonts(size_t family_id) const
{
    if (family_id < m_d->family_info.size()) {
        return m_d->family_info[family_id].contains_system_fonts;
    }
    return false;
}

bool FontManager::family_contains_db_fonts(size_t family_id) const
{
    if (family_id < m_d->family_info.size()) {
        return m_d->family_info[family_id].contains_system_fonts;
    }
    return false;
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
        const auto &info = m_d->fonts[family_id][font_id];
        return m_d->style_name(info.style_name_index);
    }
    return k_unknown_string;
}

int FontManager::style_index(size_t family_id, const std::string &name) const
{
    if (family_id < m_d->fonts.size()) {
        auto &fonts = m_d->fonts[family_id];
        int index = 0;
        int32_t style_index = m_d->style_index(name);
        for (const auto &font_info : fonts) {
            if (font_info.style_name_index == style_index) {
                return index;
            }
            index++;
        }
    }
    return 0;
}

bool FontManager::is_system(size_t family_id, size_t font_id) const
{
    if (family_id < m_d->fonts.size() && font_id < m_d->fonts[family_id].size()) {
        return !m_d->fonts[family_id][font_id].db_id.is_valid();
    }
    return false;
}

FontImpl *FontManager::font_impl(size_t family_id, size_t font_id) const
{
    if (family_id < m_d->fonts.size() && font_id < m_d->fonts[family_id].size()) {
        return m_d->fonts[family_id][font_id].typeface.get();
    }
    return nullptr;
}

void FontManager::add_typeface(sk_sp<SkTypeface> typeface,
                               const DatabaseAssetId &db_id,
                               const std::string &style)
{
    if (typeface) {
        SkString family;
        typeface->getFamilyName(&family);
        std::string family_name(family.c_str());
        uint32_t id = UINT32_MAX;
        auto &map = m_d->family_name_to_family_id;
        auto it = map.find(family_name);
        if (it == map.end()) {
            id = map.size();
            map[family_name] = id;
        } else {
            id = it->second;
        }

        const std::string style_name = !db_id.is_valid() ? skia::to_string(typeface->fontStyle())
                                                         : style;
        int32_t style_name_index = m_d->create_style_name_index(style_name);
        auto &fonts = m_d->fonts;
        if (id >= fonts.size()) {
            fonts.emplace_back();
            fonts.back().emplace_back(typeface, db_id, style_name_index);
            m_d->family_info.emplace_back(family_name);
        } else {
            fonts[id].emplace_back(typeface, db_id, style_name_index);
        }

        if (!db_id.is_valid()) {
            m_d->family_info[id].contains_system_fonts = true;
        } else {
            m_d->family_info[id].contains_db_fonts = true;
        }
        INAE_CORE_TRACE("Adding Font Family {}, Style {}", family_name, style_name);
    }
}

} // namespace inae::core
