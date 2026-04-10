#ifndef PROPERTY_VALUE_VALIDATOR_H
#define PROPERTY_VALUE_VALIDATOR_H

#include <core/utility.h>
#include <functional>
#include <limits>

namespace alive::model {

template<typename T>
struct PropertyValueValidator
{
    virtual bool is_valid(const T &) const { return true; }
    virtual bool validate(T &) const { return true; }
};

template<typename T>
struct FunctorValidator : public PropertyValueValidator<T>
{
    using FnType1 = std::function<bool(const T &)>;
    using FnType2 = std::function<bool(T &)>;

    FunctorValidator(FnType1 is_valid_fun, FnType2 validate_fun)
        : m_is_valid_fun(is_valid_fun)
        , m_validate(validate_fun)
    {}
    bool is_valid(const T &val) const { return m_is_valid_fun(val); }
    bool validate(T &val) const { return m_validate(val); }

private:
    FnType1 m_is_valid_fun;
    FnType2 m_validate;
};

template<typename T>
struct MinMaxPropertyValueValidator : public PropertyValueValidator<T>
{
    MinMaxPropertyValueValidator(T min = std::numeric_limits<T>::lowest(),
                                 T max = std::numeric_limits<T>::max())
        : m_min(min)
        , m_max(max)
    {}

    bool is_valid(const T &val) const override { return in_bound(m_min, m_max, val); }
    bool validate(T &val) const override
    {
        if (val < m_min) {
            val = m_min;
        } else if (val > m_max) {
            val = m_max;
        }

        return true;
    }

private:
    T m_min;
    T m_max;
};

template<typename T>
struct Vec2DMinPropertyValueValidator : public PropertyValueValidator<T>
{
    Vec2DMinPropertyValueValidator(T min)
        : m_min(min)
    {}

    bool is_valid(const T &val) const override
    {
        if (val.x() < m_min.x() || val.y() < m_min.y())
            return false;

        return true;
    }
    bool validate(T &val) const override
    {
        if (val.x() < m_min.x()) {
            val.x() = m_min.x();
        }

        if (val.y() < m_min.y()) {
            val.y() = m_min.y();
        }

        return true;
    }

private:
    T m_min;
};
} // namespace alive::model

#endif // PROPERTY_VALUE_VALIDATOR_H
