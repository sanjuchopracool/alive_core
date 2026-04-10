#ifndef ALIVE_CUSTOME_TYPES_H
#define ALIVE_CUSTOME_TYPES_H

#include <Corrade/Containers/Pointer.h>
#include <Magnum/Magnum.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Math/Matrix3.h>
#include <Magnum/Math/Range.h>
#include <Magnum/Math/Vector.h>
#include <Magnum/Math/Vector2.h>
#include <Magnum/Math/Vector3.h>

namespace alive {
using Vec1D = float;
using Vec2D = Magnum::Vector2;
using Point2D = Magnum::Vector2;
using Size2D = Magnum::Vector2;

using Vec3D = Magnum::Vector3;
using Rect2D = Magnum::Range2D;
using Size2Di = Magnum::Vector2i;
using Color = Magnum::Color3;
using Color4ub = Magnum::Color4ub;
using Color3ub = Magnum::Color3ub;
using Matrix3D = Magnum::Math::Matrix3<Vec1D>;

template<typename T>
using alive_ptr = Corrade::Containers::Pointer<T>;

} // namespace alive
#endif // ALIVE_CUSTOME_TYPES_H
