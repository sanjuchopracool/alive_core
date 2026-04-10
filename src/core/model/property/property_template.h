#ifndef PROPERTY_TEMPLATE_H
#define PROPERTY_TEMPLATE_H

#include "property.h"
#include <core/logging/logger.h>
#include <type_traits>

namespace alive::model {
template<typename Type>
class PropertyTemplate : public Property
{
public:
    using value_type = Type;
    using creference = const Type &;
    using reference = Type &;

    PropertyTemplate(Object *object, const PropertyDescriptor &descriptor, const std::string name)
        : Property(object, descriptor, name)
    {}

    virtual creference get_value() const = 0;
    virtual KeyFrame *set_value(creference value, KeyFrameTime t = 0) = 0;

    //    constexpr PropertyValueType get_property_value_type()
    //    {
    //        return property_value_type_from_type<Type>();
    //    }

    // scripting support
    template<typename T>
    KeyFrame *set_prop_value(const T &value, KeyFrameTime t)
    {
        if constexpr (std::is_same_v<Type, T>) {
            return set_value(value, t);
        }

        return nullptr;
    }
};
} // namespace alive::model

#endif // PROPERTY_TEMPLATE_H
