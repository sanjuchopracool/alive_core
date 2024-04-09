#include "dash_element.h"
#include <parser/lottie/property_parser.h>

#include <QJsonObject>
#include <QJsonValue>

namespace inae::core {
const QString type_key = "n";
const QString value_key = "v";

DashElement::Type from_str(const QString &str)
{
    auto result = DashElement::Type::Offset;
    if (str == "d")
        result = DashElement::Type::Dash;
    if (str == "g")
        result = DashElement::Type::Gap;

    return result;
}

DashElement::DashElement() {}

DashElement::~DashElement() {}

void DashElement::decode(const QJsonValue &in_value, QList<QString> &out_messages)
{
    QJsonObject obj = in_value.toObject();
    m_type = from_str(obj.value(type_key).toString());
    Lottie::decode(m_value, obj.value(value_key), out_messages);
}

void decode_dashes(std::vector<DashElement> &elements,
                   QJsonValue &in_value,
                   QList<QString> &out_messages)
{
    QJsonArray array = in_value.toArray();
    //    elements.resize(array.size());
    //    for (int i = 0; i < array.size(); ++i)
    //        elements[i].decode(array.at(i), out_messages);
}

} // namespace inae::core
