#ifndef SVG_IMPORTER_H
#define SVG_IMPORTER_H

#include "importer.h"

namespace alive::importer {

class SvgImporter : public Importer
{
public:
    Layers import(const std::filesystem::path &path, Messages &msgs) override;
};

} // namespace alive::importer
#endif // SVG_IMPORTER_H
