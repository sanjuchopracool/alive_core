#ifndef PROPERTY_VALUE_VALIDATOR_H
#define PROPERTY_VALUE_VALIDATOR_H

#include <limits>
#include <core/utility.h>

namespace inae::model {

template<typename T>
struct PropertyValueValidator
{
    virtual bool is_valid(const T &) const { return true; }
    virtual bool validate(T &) const { return true; }
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
        if (val.x < m_min.x || val.y < m_min.y)
            return false;

        return true;
    }
    bool validate(T &val) const override
    {
        if (val.x < m_min.x) {
            val.x = m_min.x;
        }

        if (val.y < m_min.y) {
            val.y = m_min.y;
        }

        return true;
    }

private:
    T m_min;
};
} // namespace inae::model

#endif // PROPERTY_VALUE_VALIDATOR_H
