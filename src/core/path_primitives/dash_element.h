#ifndef DASHELEMENT_H
#define DASHELEMENT_H

#include <model/property_system/property.h>
#include <vector>
#include <QJsonArray>

class QJsonValue;

namespace inae::core {
struct DashElementData;

struct DashElement
{
    DashElement();
    ~DashElement();
    enum Type
    {
        Offset,
        Dash,
        Gap
    };

    Type m_type;
    PropertyPtr<Vector1D> m_value;

    void decode(const QJsonValue &in_value, QList<QString> &out_messages);
};

void decode_dashes(std::vector<DashElement> &elements,
                   QJsonValue &in_value,
                   QList<QString> &out_messages);
} // namespace inae::core

#endif // DASHELEMENT_H
