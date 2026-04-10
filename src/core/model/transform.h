#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <core/model/property/final_properties.h>
#include <core/model/property/object.h>
#include <core/serializer/serializer_common.h>

namespace alive::core {
class Serializer;
}

namespace alive::model {
class Transform : public Object
{
    ADD_ANCHOR_3D
    ADD_POSITION_3D
    ADD_SCALE_3D
    ADD_ROTATION
    ADD_OPACITY
    ADD_SKEW
    ADD_SKEW_AXIS
public:
    Transform(Object *object = nullptr);
    Transform(Object *object, const Transform &other);

    Transform *clone(Object *object) const override;

private:
    ADD_FRIEND_SERIALIZERS
};
} // namespace alive::model

#endif // TRANSFORM_H
