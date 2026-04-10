#ifndef KEYFRAME_UTILITY_H
#define KEYFRAME_UTILITY_H

namespace alive::core {

template<typename T>
T remap(T value, T from_low, T from_high, T to_low, T to_high)
{
    return to_low + (value - from_low) * (to_high - to_low) / (from_high - from_low);
}

template<typename T>
T interpolate(const T& from,const T& to, float amount)
{
    return  from + (to - from) * amount;
}

} // namespace alive::core

#endif // KEYFRAME_UTILITY_H
