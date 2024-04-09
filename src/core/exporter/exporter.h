#ifndef EXPORTER_H
#define EXPORTER_H

#include <core/logging/logger.h>
#include <string>

namespace inae::model {
class Composition;
}

namespace inae::exporter {
using namespace inae::model;
class Exporter
{
public:
    Exporter(const Composition *comp);

    enum Format { e_PNGSequence, e_WebM };
    bool export_as(const std::string &path, Format format = e_PNGSequence);

private:
    const Composition *m_comp;
};

} // namespace inae::exporter

#endif // EXPORTER_H
