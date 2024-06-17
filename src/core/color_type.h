#ifndef COLOR_TYPE_H
#define COLOR_TYPE_H

#include "common_types.h"
#include <include/core/SkColor.h>
namespace inae {

/**
 * @brief The Color class is wrapper over skcolor adapted to inae
 */

inline SkColor skcolor_from_vec3d(Vec3D value)
{
    if (value.x() <= 1 && value.y() <= 1 && value.z() <= 1) {
        return SkColorSetARGB(255, value.x() * 255, value.y() * 255, value.z() * 255);
    }
    return SkColorSetARGB(255, value.x(), value.y(), value.z());
}

inline Vec3D skcolor_to_vec3d(SkColor c)
{
    Vec3D val;
    val.x() = SkColorGetR(c) / 255.0;
    val.y() = SkColorGetG(c) / 255.0;
    val.z() = SkColorGetB(c) / 255.0;
    return val;
}

std::string skcolor_to_rgb_string(SkColor color);

inline SkColor skcolor_from_rgb_string(const std::string &str)
{
    uint32_t color = static_cast<uint32_t>(std::stoll("0x" + str.substr(1), nullptr, 16));
    return (color | (0xFF << 24));
}

// class Color
// {
// public:
//     Color(const Vec3D &in = Vec3D())
//         : m_data(in)
//     {}

//     Color operator+(const Color &other) const { return {m_data + other.m_data}; }
//     Color operator-(const Color &other) const { return {m_data - other.m_data}; }
//     Color operator*=(SkScalar s) { return {m_data * s}; }

// private:
//     Vec3D m_data{};
// };

} // namespace inae
#endif // COLOR_TYPE_H
