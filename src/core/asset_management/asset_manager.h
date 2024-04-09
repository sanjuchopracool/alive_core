#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <boost/noncopyable.hpp>
#include <memory>

namespace inae::core {

class FontManager;
struct AssetManagerPrivateData;
class AssetManager : public boost::noncopyable
{
public:
    AssetManager();
    ~AssetManager();

private:
    std::unique_ptr<AssetManagerPrivateData> m_d;
};
} // namespace inae::core

#endif // ASSETMANAGER_H
