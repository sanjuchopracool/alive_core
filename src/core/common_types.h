#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#include <assert.h>
#include <core/inae_types/types.h>
#include <include/core/SkM44.h>
#include <string>
#include <vector>

#define assertm(exp, msg) assert(((void)msg, exp))

using SerializerWarnings = std::vector<std::string>;
namespace inae {
class Version;
namespace model {
enum class LayerType { e_Precomp, e_Solid, e_Image, e_Null, e_Shape, e_Text };
}
enum class PathDirection { e_None = 0, e_Clockwise = 1, e_UserSetClockwise, e_CounterClockwise };

inline const std::string &empty_string()
{
    static std::string str = "";
    return str;
}

constexpr float k_null_float_value = 0.0f;
using FrameTimeType = float;
using KeyFrameTime = int;

inline bool is_null(const Vec2D &val)
{
    return val.x == k_null_float_value && val.y == k_null_float_value;
}

inline bool is_null(const Vec3D &val)
{
    return val.x == k_null_float_value && val.y == k_null_float_value
           && val.z == k_null_float_value;
}
inline Vec3D operator/(Vec3D v, SkScalar s)
{
    return {v.x / s, v.y / s, v.z / s};
}

enum class CoordinateSpace {
    e_Type2D,
    e_Type3D,
};

} // namespace inae

#endif // LOTTIELIB_H
