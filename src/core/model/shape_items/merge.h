#ifndef MERGE_H
#define MERGE_H

#include "shape_item.h"

namespace inae::core {

enum class MergeMode {
      None,
      Merge,
      Add,
      Subtract,
      Intersect,
      Exclude
};

class Merge : public ShapeItem
{
public:
    Merge();

    virtual void decode(QJsonObject& in_obj, QList<QString>& out_messages) override;

private:
    MergeMode m_mode = MergeMode::None;
};
}

#endif // MERGE_H
