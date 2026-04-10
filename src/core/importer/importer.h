#ifndef IMPORTER_H
#define IMPORTER_H

#include <Corrade/Containers/Pointer.h>
#include <core/serializer/message.h>
#include <filesystem>

namespace alive::model {
class Layer;
}

using LayerPtr = Corrade::Containers::Pointer<alive::model::Layer>;
using Layers = std::vector<LayerPtr>;

namespace alive::importer {
class Importer
{
public:
    virtual ~Importer() {}
    virtual Layers import(const std::filesystem::path &path, Messages &msgs) = 0;
};

Layers import_file(const std::filesystem::path &path, Messages &msgs);

} // namespace alive::importer

#endif // IMPORTER_H
