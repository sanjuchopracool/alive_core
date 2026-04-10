#ifndef PROPERTY_ANIMATOR_H
#define PROPERTY_ANIMATOR_H

#include <core/alive_types/common_types.h>
#include <memory>

namespace alive::model {
class UpdateContext;

template<typename T>
class PropertyAnimator
{
public:
    using CRefValueType = const T &;
    virtual ~PropertyAnimator() {}
    virtual void update(UpdateContext &context) = 0;
    virtual CRefValueType value() const = 0;
    virtual void notify() = 0;
    virtual bool is_valid_expression(script::ExpressionEngineInterface *engine,
                                     const std::string &expression)
        = 0;
};

template<typename T>
using PropertyAnimatorPtr = PropertyAnimator<T> *;

template<typename T>
using PropertyAnimatorUniquePtr = std::unique_ptr<PropertyAnimator<T>>;

} // namespace alive::model
#endif // PROPERTY_ANIMATOR_H
