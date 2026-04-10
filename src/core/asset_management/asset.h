#ifndef ASSET_H
#define ASSET_H

#include "database.h"
#include <core/empty_values.h>
#include <string>
#include <variant>

namespace alive::core {

using AssetId = std::variant<DatabaseAssetId, std::string>;
class Asset
{
public:
    Asset(AssetType type, AssetId id)
        : m_type(type)
        , m_id(id)
    {}
    AssetType type() const { return m_type; }

    bool is_local() const;
    const std::string &path() const;
    const DatabaseAssetId db_id() const;

private:
    AssetId m_id;
    AssetType m_type;
};

} // namespace alive::core
#endif // ASSET_H
