#ifndef UTILITY_H
#define UTILITY_H

namespace inae {

template<typename T>
bool in_bound(T min, T max, T val)
{
    return val >= min && val <= max;
}
} // namespace inae

#endif // UTILITY_H
