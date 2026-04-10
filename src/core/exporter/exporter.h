#ifndef EXPORTER_H
#define EXPORTER_H

#include <core/logging/logger.h>
#include <string>

namespace alive::model {
class Composition;
}

namespace alive::exporter {
using namespace alive::model;
class Exporter
{
public:
    Exporter(const Composition *comp);

    enum Format { e_PNGSequence, e_WebM };
    bool export_as(const std::string &path, Format format = e_PNGSequence, float scale = 1.0f);

private:
    const Composition *m_comp;
};

} // namespace alive::exporter

#endif // EXPORTER_H
