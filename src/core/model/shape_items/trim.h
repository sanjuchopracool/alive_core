#ifndef TRIM_H
#define TRIM_H

#include <core/model/property/trim_properties.h>
#include <core/model/shape_items/shape_item.h>

namespace alive::model {

class Trim : public ShapeItem
{
    ADD_TRIM_START
    ADD_TRIM_END
    ADD_TRIM_OFFSET
public:
    Trim(Object *object = nullptr);
    enum class TrimType { e_Simultaneously = 1, e_Individually = 2 };

    auto trim_type() const { return m_trimType; }

    Trim(Object *object, const Trim &other);
    Trim *clone_shape_item(Object *object) const override;

public:
    TrimType m_trimType = TrimType::e_Simultaneously;

private:
    ADD_FRIEND_SERIALIZERS
};
} // namespace alive::model

#endif // TRIM_H
