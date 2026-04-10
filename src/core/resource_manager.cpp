#include "resource_manager.h"
#include <core/asset_management/font_manager.h>
#include <core/profiler.h>
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

#include <core/asset_management/asset_manager.h>
#include <core/logging/logger.h>
#include <include/utils/SkCustomTypeface.h>

namespace alive::ResourceManager {
static sk_sp<SkFontMgr> font_mgr;

void init_from_path(const std::string& path)
{
    {
        alive::AutoProfiler p("Time Taken to load FontManager");
#ifdef _WINDOWS
        font_mgr.reset(SkFontMgr_New_DirectWrite().release());
#elif defined(__APPLE__)
        font_mgr = SkFontMgr_New_CoreText(nullptr);
#else
        font_mgr.reset(SkFontMgr_New_Custom_Directory("/usr/share/fonts/").release());
#endif
        ALIVE_CORE_INFO("No of Font's Loaded: {}", font_mgr->countFamilies());
    }

    core::FontManager::init();
    // core::FontManager::instance().load_system_fonts();
    core::AssetManager::init();
    core::AssetManager::instance().load_file_databse(path);
}

const SkFontMgr &system_font_manager()
{
    return *font_mgr.get();
}

} // namespace alive::ResourceManager
