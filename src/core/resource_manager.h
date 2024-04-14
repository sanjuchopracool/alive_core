#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <string>

class SkFontMgr;

namespace inae::ResourceManager {

void init_from_path(const std::string& path);
const SkFontMgr &system_font_manager();

} // namespace inae::ResourceManager

#endif // RESOURCEMANAGER_H
