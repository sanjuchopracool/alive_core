#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#include <assert.h>
#include <core/alive_types/types.h>
#include <string>

#define assertm(exp, msg) assert(((void)msg, exp))

namespace alive {
class Version;
namespace model {
enum class LayerType { e_Precomp, e_Solid, e_Image, e_Null, e_Shape, e_Text };
}
enum class PathDirection { e_None = 0, e_Clockwise = 1, e_UserSetClockwise, e_CounterClockwise };
enum class Direction {e_Left, e_Right, e_Top, e_Bottom};
enum class EditingField {
    e_None,
    e_First,
    e_Second,
};

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
    return val.x() == k_null_float_value && val.y() == k_null_float_value;
}

inline bool is_null(const Vec3D &val)
{
    return val.x() == k_null_float_value && val.y() == k_null_float_value
           && val.z() == k_null_float_value;
}

enum class CoordinateSpace {
    e_Type2D,
    e_Type3D,
};

} // namespace alive

#endif // LOTTIELIB_H
