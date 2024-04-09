#ifndef ELLIPSE_H
#define ELLIPSE_H

#include <core/model/property/final_properties.h>
#include <core/model/shape_items/shape_item.h>

namespace inae::core {
class Serializer;
}

namespace inae::model {

class Ellipse : public PathShapeItem
{
    ADD_SIZE_2D
    ADD_POSITION_2D
public:
    Ellipse(Object *object = nullptr);

private:
    friend class inae::core::Serializer;
};
} // namespace inae::model

#endif // ELLIPSE_H
