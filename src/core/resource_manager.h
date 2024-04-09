#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <string_view>

class SkFontMgr;

namespace inae::ResourceManager {

void init_from_path(std::string_view path);
const SkFontMgr &system_font_manager();

} // namespace inae::ResourceManager

#endif // RESOURCEMANAGER_H
