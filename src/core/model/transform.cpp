#include "transform.h"
namespace inae::model {

const ObjectDescriptor &transform_descriptor()
{
    static auto transform_descriptor = ObjectDescriptor("Transform", ObjectDescriptor::e_Transform);
    transform_descriptor.set_support_visibility(false);
    return transform_descriptor;
}
Transform::Transform(Object *object)
    : Object(object, transform_descriptor())
{}
} // namespace inae::model
