#include "asset.h"
namespace inae::core {

bool Asset::is_local() const
{
    return std::holds_alternative<std::string>(m_id);
}

const std::string &Asset::path() const
{
    const std::string *str = std::get_if<std::string>(&m_id);
    if (str) {
        return *str;
    }
    return inae::k_empty_string;
}

const DatabaseAssetId Asset::db_id() const
{
    {
        const DatabaseAssetId *db_id = std::get_if<DatabaseAssetId>(&m_id);
        if (db_id) {
            return *db_id;
        }
        return k_invalid_db_id;
    }
}

} // namespace inae::core
