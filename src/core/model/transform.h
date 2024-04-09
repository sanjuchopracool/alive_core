#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <core/model/property/final_properties.h>
#include <core/model/property/object.h>

namespace inae::core {
class Serializer;
}

namespace inae::model {
class Transform : public Object
{
    ADD_ANCHOR_3D
    ADD_POSITION_3D
    ADD_SCALE_3D
    ADD_ROTATION
    ADD_OPACITY

public:
    Transform(Object *object = nullptr);

private:
    friend class inae::core::Serializer;
};
} // namespace inae::model

#endif // TRANSFORM_H
