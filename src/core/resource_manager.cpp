#include "resource_manager.h"
#include <core/asset_management/font_manager.h>
#include <filesystem>
#include <include/core/SkFontMgr.h>
#include <include/core/SkStream.h>
#include <include/core/SkTypeface.h>

#ifdef _WINDOWS
#include <include/ports/SkTypeface_win.h>
#elif __APPLE__
#include <include/ports/SkFontMgr_mac_ct.h>
#else
#include <include/ports/SkFontMgr_directory.h>
#endif

#include <include/utils/SkCustomTypeface.h>
#include <core/logging/logger.h>

namespace inae::ResourceManager {
constexpr auto k_fonts_suffix = "fonts";
sk_sp<SkFontMgr> font_mgr;

void init_from_path(std::string_view path)
{
    using namespace std::filesystem;
    std::error_code err{};
    const std::filesystem::path resource_dir{path};
    if (!std::filesystem::exists(resource_dir, err)) {
        throw filesystem_error(fmt::format("resource directory {} does not exist!", path), err);
    }
    {
#ifdef _WINDOWS
        font_mgr.reset(SkFontMgr_New_GDI().release());
#elif __APPLE__
        font_mgr.reset(SkFontMgr_New_CoreText(nullptr).release());
#else
        font_mgr.reset(SkFontMgr_New_Custom_Directory("/usr/share/fonts/").release());
#endif
        INAE_CORE_INFO("No of Font's Loaded: {}", font_mgr->countFamilies());
    }

    core::FontManager::init();
    auto fonts_path = resource_dir / "fonts";
    core::FontManager::instance().load_system_fonts();
    core::FontManager::instance().load_fonts_from_path(fonts_path);
}

const SkFontMgr &system_font_manager()
{
    return *font_mgr.get();
}

} // namespace inae::ResourceManager
