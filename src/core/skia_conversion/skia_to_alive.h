#ifndef SKIA_TO_ALIVE_H
#define SKIA_TO_ALIVE_H

#include <core/alive_types/color_type.h>
#include <core/stroke_types.h>
#include <include/core/SkMatrix.h>
#include <include/core/SkPaint.h>
#include <string>

class SkFontStyle;

namespace alive::skia {
std::string to_string(const SkFontStyle &style);
inline alive::CapStyle from_sk_cap_style(SkPaint::Cap cap)
{
    switch (cap) {
    case SkPaint::kButt_Cap:
        return alive::CapStyle::e_Butt;
        break;
    case SkPaint::kRound_Cap:
        return alive::CapStyle::e_Round;
        break;
    case SkPaint::kSquare_Cap:
        return alive::CapStyle::e_Square;
        break;
    default:
        break;
    }
    return alive::CapStyle::e_Butt;
}
inline SkPaint::Cap to_sk_cap_style(alive::CapStyle cap)
{
    switch (cap) {
    case alive::CapStyle::e_Butt:
        return SkPaint::kButt_Cap;
        break;
    case alive::CapStyle::e_Round:
        return SkPaint::kRound_Cap;
        break;
    case alive::CapStyle::e_Square:
        return SkPaint::kSquare_Cap;
        break;
    default:
        break;
    }
    return SkPaint::kButt_Cap;
}

inline alive::JoinStyle from_sk_join_style(SkPaint::Join cap)
{
    switch (cap) {
    case SkPaint::kMiter_Join:
        return alive::JoinStyle::e_Miter;
        break;
    case SkPaint::kRound_Join:
        return alive::JoinStyle::e_Round;
        break;
    case SkPaint::kBevel_Join:
        return alive::JoinStyle::e_Bevel;
        break;
    default:
        break;
    }
    return alive::JoinStyle::e_Miter;
}
inline SkPaint::Join to_sk_join_style(alive::JoinStyle cap)
{
    switch (cap) {
    case alive::JoinStyle::e_Miter:
        return SkPaint::kMiter_Join;
        break;
    case alive::JoinStyle::e_Round:
        return SkPaint::kRound_Join;
        break;
    case alive::JoinStyle::e_Bevel:
        return SkPaint::kBevel_Join;
        break;
    default:
        break;
    }
    return SkPaint::kMiter_Join;
}
inline SkColor to_sk_color(const Color4ub &color)
{
    return SkColorSetARGB(color.a(), color.r(), color.g(), color.b());
}
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

SkMatrix to_skmatrix(const Matrix3D &mat);
}
#endif // SKIA_TO_ALIVE_H
