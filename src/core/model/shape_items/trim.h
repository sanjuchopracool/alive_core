#ifndef TRIM_H
#define TRIM_H

#include <core/model/property/trim_properties.h>
#include <core/model/shape_items/shape_item.h>

namespace inae::model {

class Trim : public ShapeItem
{
    ADD_TRIM_START
    ADD_TRIM_END
    ADD_TRIM_OFFSET
public:
    Trim(Object *object = nullptr);
    enum class TrimType { Simultaneously = 1, Individually = 2 };

    auto trim_type() const { return m_trimType; }

public:
    TrimType m_trimType = TrimType::Simultaneously;

private:
    friend class inae::core::Serializer;
};
} // namespace inae::model

#endif // TRIM_H
