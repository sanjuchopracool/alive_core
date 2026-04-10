#ifndef STATIC_PROPERTY_H
#define STATIC_PROPERTY_H

#include "property_template.h"
#include "property_value_validator.h"

namespace alive::model {
template<typename T>
class StaticProperty : public PropertyTemplate<T>
{
public:
    using creference = const T &;
    using reference = T &;

    StaticProperty(Object *object,
                   const PropertyDescriptor &descriptor,
                   const PropertyValueValidator<T> *validator = nullptr,
                   const T &default_value = {},
                   const std::string name = {})
        : PropertyTemplate<T>(object, descriptor, name)
        , m_validator(validator)
    {
        set_value(default_value);
    }

    KeyFrame *set_value(creference value, KeyFrameTime t = 0) override final
    {
        T validated_value = value;
        if (m_validator) {
            m_validator->validate(validated_value);
        }

        if (m_value != validated_value) {
            m_value = validated_value;
            Property::notify_observers();
        }

        return nullptr;
    }

    bool assign_value_from(const Property *other) override
    {
        if (other && other->type() == this->type()) {
            const auto *sp_other = static_cast<const StaticProperty<T> *>(other);
            if (sp_other) {
                set_value(sp_other->get_value());
                return true;
            }
        }
        return false;
    }
    creference get_value() const override { return m_value; }
    ~StaticProperty() {}

    Property *clone(Object *obj) const override { return new StaticProperty(obj, *this); }

    StaticProperty(Object *object, const StaticProperty &other)
        : PropertyTemplate<T>(object, other.m_descriptor, other.name())
        , m_value(other.m_value)
        , m_validator(other.m_validator)
    {}

protected:
    T m_value = {};
    const PropertyValueValidator<T> *m_validator = nullptr;
};

template<typename T>
StaticProperty<T> *to_static_property(Property *p)
{
    return static_cast<StaticProperty<T> *>(p);
}

template<typename T>
class StaticPropertyWithReset : public StaticProperty<T>
{
public:
    using creference = const T &;
    using reference = T &;

    StaticPropertyWithReset(Object *object,
                            const PropertyDescriptor &descriptor,
                            const PropertyValueValidator<T> *validator = nullptr,
                            const T &default_value = {},
                            const std::string name = {})
        : StaticProperty<T>(object, descriptor, validator, default_value, name)
    {}

    void update_with_actual_value(const T &data)
    {
        m_reset_value = StaticProperty<T>::m_value;
        StaticProperty<T>::set_value(data);
    }

private:
    T m_reset_value = {};
};
} // namespace alive::model
#endif // STATIC_PROPERTY_H
