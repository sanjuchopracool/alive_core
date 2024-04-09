#ifndef INAE_CUSTOME_TYPES_H
#define INAE_CUSTOME_TYPES_H

#include <include/core/SkM44.h>
#include <string_view>

namespace inae {
using Vec1D = float;
using Vec2D = SkV2;
using Vec3D = SkV3;
using Point = SkPoint;
using Rect = SkRect;
using Size = SkSize;

inline Vec3D create_vec3d(Vec1D x = 0.f, Vec1D y = 0.f, Vec1D z = 0.f)
{
    return SkV3{x, y, z};
}

inline Vec2D create_vec2d(Vec1D x = 0.f, Vec1D y = 0.0f)
{
    return SkV2{x, y};
}

} // namespace inae
#endif // INAE_CUSTOME_TYPES_H
