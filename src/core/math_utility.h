#ifndef MATH_UTILITY_H
#define MATH_UTILITY_H

#include <cmath>

namespace alive::math {

#define FLOAT_EPSILON 1.0e-06f //1.192092896e-07f

static inline bool zero(float a)
{
    return (fabsf(a) <= FLOAT_EPSILON) ? true : false;
}
} // namespace alive::math
#endif // MATH_UTILITY_H
