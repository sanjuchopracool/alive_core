#include "asset_manager.h"
#include "file_database.h"
#include <memory>
#include <vector>

namespace alive::core {

std::unique_ptr<AssetManager> asset_mgr;

struct AssetManagerPrivateData
{
    std::vector<std::unique_ptr<Database>> databases;
};

AssetManager::AssetManager()
    : m_d(std::make_unique<AssetManagerPrivateData>())
{}

AssetManager::~AssetManager() {}

void AssetManager::load_file_databse(const std::string &path)
{
    m_d->databases.emplace_back(
        std::make_unique<FileDatabase>(path, "CHECK", static_cast<DataBaseId>(m_d->databases.size())));
}

void AssetManager::init()
{
    if (!asset_mgr) {
        asset_mgr.reset(new AssetManager());
    }
}

AssetManager &AssetManager::instance()
{
    return *asset_mgr.get();
}
} // namespace alive::core
