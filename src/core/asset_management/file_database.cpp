#include "file_database.h"
#include "font_manager.h"
#include <boost/algorithm/string.hpp>
#include <core/logging/logger.h>
#include <core/resource_manager.h>
#include <filesystem>
#include <fstream>
#include <include/core/SkFontMgr.h>
#include <include/core/SkTypeface.h>
#include <vector>

namespace inae::core {
FileDatabase::FileDatabase(const std::string &path, const std::string &name, DataBaseId db_id)
    : Database(name, db_id)
    , m_path(path)
{
    using namespace std::filesystem;
    std::error_code err{};
    const std::filesystem::path resource_dir{path};
    if (!std::filesystem::exists(resource_dir, err)) {
        throw filesystem_error(fmt::format("Database directory {} does not exist!", path), err);
    }

    auto fonts_db_path = resource_dir / "fonts.db";
    if (std::filesystem::exists(fonts_db_path, err)) {
        std::ifstream file(fonts_db_path);
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                std::vector<std::string> strs;
                boost::split(strs, line, boost::is_any_of(";"));
                if (strs.size() == 2) {
                    m_font_id_to_path[std::stoi(strs[1])] = strs[0];
                } else {
                    INAE_CORE_ERROR("Invalid entry in fonts.db: {}", line);
                }
            }
            file.close();
        } else {
            throw std::runtime_error(fmt::format("failed to open file {}!", path));
        }
        init_fonts();
    }
}

sk_sp<SkData> FileDatabase::data(AssetType type, DbItemId)
{
    sk_sp<SkData> result;
    return result;
}

void FileDatabase::init_fonts()
{
    std::string db_path = m_path + '/';
    for (const auto &font : m_font_id_to_path) {
        std::string path = db_path + font.second;
        auto typeface = ResourceManager::system_font_manager().makeFromFile(path.c_str());
        FontManager::instance().add_typeface(typeface, {m_id, font.first});
    }
}
} // namespace inae::core
