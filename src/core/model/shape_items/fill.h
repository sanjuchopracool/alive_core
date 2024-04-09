#ifndef FILL_H
#define FILL_H

#include "shape_properties_macros.h"
#include <core/model/property/final_properties.h>
#include <core/model/shape_items/shape_item.h>

namespace inae::core {
class Serializer;
}

namespace inae::model {
class Fill : public ShapeItem
{
    ADD_COLOR
    ADD_OPACITY
public:
    enum class FillRule { e_None, e_NonZeroWinding, e_EvenOdd };

    Fill(Object *object = nullptr);

    //    virtual void decode(QJsonObject& in_obj, QList<QString>& out_messages) override;

public:
    FillRule m_fill_rule = FillRule::e_None;
    bool m_fillEnabled = true;

private:
    friend class inae::core::Serializer;
};
} // namespace inae::model

#endif // FILL_H
