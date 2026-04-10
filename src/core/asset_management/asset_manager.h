#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <boost/noncopyable.hpp>
#include <memory>
#include <string>

namespace alive::core {

class FontManager;
struct AssetManagerPrivateData;
class AssetManager : public boost::noncopyable
{
public:
    ~AssetManager();
    void load_file_databse(const std::string &path);

    static void init();
    static AssetManager &instance();

private:
    AssetManager();
    std::unique_ptr<AssetManagerPrivateData> m_d;
};
} // namespace alive::core

#endif // ASSETMANAGER_H
