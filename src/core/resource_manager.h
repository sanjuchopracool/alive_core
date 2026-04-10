#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <string>

class SkFontMgr;

namespace alive::ResourceManager {

void init_from_path(const std::string& path);
const SkFontMgr &system_font_manager();

} // namespace alive::ResourceManager

#endif // RESOURCEMANAGER_H
