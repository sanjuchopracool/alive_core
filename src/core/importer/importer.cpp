#include "importer.h"
#include "svg_importer.h"
#include <core/model/layers/layer.h>
#include <format>
#include <map>

namespace {
std::map<std::string, Corrade::Containers::Pointer<alive::importer::Importer>> importers;
}
namespace alive::importer {

Layers import_file(const std::filesystem::path &path, Messages &msgs)
{
    // TODO init
    if (!importers.size()) {
        importers[".svg"] = Corrade::Containers::pointer<>(new SvgImporter());
    }

    Layers layers;
    if (!std::filesystem::is_regular_file(path) || !std::filesystem::exists(path)) {
        msgs.emplace_back(MessageType::e_Error,
                          std::format("Please check if file {} exist!", path.string()));
        return layers;
    }

    auto ext = path.extension();
    if (ext.empty()) {
        msgs.emplace_back(MessageType::e_Error,
                          std::format("Importer are selected based on extension, file {} does not "
                                      "contain any extension.!",
                                      path.string()));
        return layers;
    }

    auto it = importers.find(ext.string());
    if (it == importers.end()) {
        msgs.emplace_back(MessageType::e_Error,
                          std::format("Could not find importer for extension {}.", path.string()));
        return layers;
    }

    return it->second->import(path, msgs);
}

} // namespace alive::importer
