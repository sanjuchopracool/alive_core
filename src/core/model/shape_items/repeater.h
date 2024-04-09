#ifndef REPEATER_H
#define REPEATER_H

#include "repeater_properties.h"
#include "shape_transformation.h"
#include <core/model/property_system/property.h>

namespace inae::core {
class Serializer;
}
namespace inae::model {

class Repeater : public ShapeItem
{
    ADD_COPIES
    ADD_OFFSET

public:
    Repeater(Object *object = nullptr);
    const ShapeTransformation *transform() const { return &m_transformation; }

public:
    ShapeTransformation m_transformation;
    int m_composite = 1;

private:
    friend class inae::core::Serializer;
};
} // namespace inae::model

#endif // REPEATER_H
