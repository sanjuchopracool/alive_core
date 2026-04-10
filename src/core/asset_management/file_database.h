#ifndef FILEDATABASE_H
#define FILEDATABASE_H

#include "database.h"
#include <map>

namespace alive::core {
class FileDatabase : public Database
{
public:
    FileDatabase(const std::string &path, const std::string &name, DataBaseId db_id);
    sk_sp<SkData> data(AssetType type, DbItemId) override;

private:
    void init_fonts();

private:
    const std::string m_path;
    std::map<DataBaseId, std::string> m_font_id_to_path;
};
} // namespace alive::core

#endif // FILEDATABASE_H
