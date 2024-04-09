#ifndef SHAPETRANSFORMATION_H
#define SHAPETRANSFORMATION_H

#include "shape_properties_macros.h"
#include <core/model/property/final_properties.h>
#include <core/model/shape_items/shape_item.h>

namespace inae::core {
class Serializer;
}

namespace inae::model {

class ShapeTransformation : public ShapeItem
{
    ADD_ANCHOR_2D
    ADD_POSITION_2D
    ADD_SCALE_2D
    ADD_ROTATION
    ADD_OPACITY
    ADD_SKEW
    ADD_SKEW_AXIS
public:
    ShapeTransformation(Object *object = nullptr);

private:
    friend class inae::core::Serializer;
};

} // namespace inae::model

#endif // SHAPETRANSFORMATION_H
