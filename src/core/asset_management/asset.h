#ifndef ASSET_H
#define ASSET_H

#include <cstdint>

namespace inae::core {

enum class AssetType { e_Font, e_Image };
constexpr uint32_t k_invalid_asset_id = UINT32_MAX;

class Asset
{
public:
    Asset(AssetType type, bool in_local_db = true, uint32_t id = k_invalid_asset_id)
        : m_type(type)
        , m_local_db(in_local_db)
        , m_id(k_invalid_asset_id)
    {}

    bool is_local() const { return m_id == k_invalid_asset_id; }
    bool in_local_db() const { return m_local_db; }
    uint32_t id() const { return m_id; }

private:
    bool m_local_db;
    uint32_t m_id;
    AssetType m_type;
};

} // namespace inae::core
#endif // ASSET_H
