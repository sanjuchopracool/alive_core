#ifndef PROPERTY_H
#define PROPERTY_H

#include "property_update_listener.h"
#include <common_types.h>
#include <memory>

namespace inae::core {

template<typename T>
class PropertyAnimator
{
public:
    using CRefValueType = const T &;
    virtual ~PropertyAnimator() {}
    virtual void update(inae::core::FrameType frame) = 0;
    virtual CRefValueType value() const = 0;
};

template<typename T>
class Property
{
public:
    using ValueType = T;
    using RefValueType = T &;
    using CRefValueType = const T &;

    virtual ~Property() {}

    virtual std::unique_ptr<PropertyAnimator<T>> create_animator(
        PropertyUpdateListener *listener) const
        = 0;
};

template<typename T>
using PropertyPtr = std::unique_ptr<Property<T>>;

template<typename T>
using PropertyAnimatorPtr = std::unique_ptr<PropertyAnimator<T>>;
} // namespace inae::core

#endif // PROPERTY_H
