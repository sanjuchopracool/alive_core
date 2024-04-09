#include "asset_manager.h"

namespace inae::core {
struct AssetManagerPrivateData
{};

AssetManager::AssetManager()
    : m_d(std::make_unique<AssetManagerPrivateData>())
{}
} // namespace inae::core
