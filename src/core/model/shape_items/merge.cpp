#include "merge.h"

#include <QJsonObject>

namespace inae::core {

const QString mode_key("mm");

Merge::Merge()
    : ShapeItem(ShapeType::Merge)
{}

void Merge::decode(QJsonObject &in_obj, QList<QString> &out_messages)
{
    m_mode = static_cast<MergeMode>(in_obj.take(mode_key).toInt());
}

}
