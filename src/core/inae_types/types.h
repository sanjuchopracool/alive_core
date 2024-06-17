#ifndef INAE_CUSTOME_TYPES_H
#define INAE_CUSTOME_TYPES_H

#include <Magnum/Magnum.h>
#include <Magnum/Math/Vector.h>
#include <Magnum/Math/Vector2.h>
#include <Magnum/Math/Vector3.h>

namespace inae {
using Vec1D = float;
using Vec2D = Magnum::Vector2;
using Vec3D = Magnum::Vector3;
using Point = Magnum::Vector2;
using Rect = Magnum::Vector4;
using Size = Magnum::Vector2;

inline Vec3D create_vec3d(Vec1D x = 0.f, Vec1D y = 0.f, Vec1D z = 0.f)
{
    return Vec3D{x, y, z};
}

inline Vec2D create_vec2d(Vec1D x = 0.f, Vec1D y = 0.0f)
{
    return Vec2D{x, y};
}

} // namespace inae
#endif // INAE_CUSTOME_TYPES_H
