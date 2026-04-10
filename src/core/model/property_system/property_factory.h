#ifndef PROPERTY_FACTORY_H
#define PROPERTY_FACTORY_H

#include "constant_property.h"
#include "keyframe_property.h"

namespace inae::core {
template<typename T>
std::unique_ptr<Property<T>> create_constant_property(T value)
{
    return std::make_unique<ConstantProperty<T>>(value);
}

template<typename T>
std::unique_ptr<Property<T>> create_keyframe_property(KeyFrames<T> &frames)
{
    return std::make_unique<KeyFrameProperty<T>>(frames);
}

} // namespace inae::core

#endif // PROPERTY_FACTORY_H
