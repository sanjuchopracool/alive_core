#ifndef STROKE_H
#define STROKE_H

#include "shape_properties_macros.h"
#include <core/model/property/final_properties.h>
#include <core/model/shape_items/shape_item.h>

namespace inae::model {

class Stroke : public ShapeItem
{
    ADD_WIDTH
    ADD_OPACITY
    ADD_COLOR
public:
    enum class StrokeRule { e_None, e_NonZeroWinding, e_EvenOdd };

    Stroke(Object *object = nullptr);

    //    virtual void decode(QJsonObject& in_obj, QList<QString>& out_messages) override;

public:
    //    PropertyPtr<std::vector<DashElement>> m_dashPattern;
    bool m_fillEnabled = false;

private:
    friend class inae::core::Serializer;
};
} // namespace inae::model

#endif // STROKE_H
