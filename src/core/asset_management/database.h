#ifndef DATABASE_H
#define DATABASE_H

#include <compare>
#include <cstdint>
#include <include/core/SkData.h>
#include <limits>
#include <string>

namespace inae::core {
using DataBaseId = uint32_t;
constexpr DataBaseId k_invalid_database_id = std::numeric_limits<DataBaseId>::max();

using DbItemId = uint32_t;
constexpr DbItemId k_invalid_db_item_id = std::numeric_limits<DbItemId>::max();

enum class AssetType { e_Font, e_Image };

struct DatabaseAssetId
{
    DataBaseId db_id = k_invalid_database_id;
    DbItemId item_id = k_invalid_db_item_id;
    bool operator==(const DatabaseAssetId &other) const = default;
    auto operator<=> (const DatabaseAssetId &oth) const = default;

    bool is_valid() const
    {
        return db_id != k_invalid_database_id && item_id != k_invalid_db_item_id;
    }
};

class Database
{
public:
    Database(const std::string &name, DataBaseId id);
    virtual ~Database();
    virtual sk_sp<SkData> data(AssetType type, DbItemId) = 0;

protected:
    const std::string m_name;
    const DataBaseId m_id;
};

} // namespace inae::core

#endif // DATABASE_H
