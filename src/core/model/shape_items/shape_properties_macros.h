#ifndef SHAPE_PROPERTIES_MACROS_H
#define SHAPE_PROPERTIES_MACROS_H

#include <core/color_type.h>
#include <core/model/property/property_macros.h>
#include <core/model/property/property_type.h>
#include <core/path_primitives/bezier_path.h>

namespace inae::model {

constexpr Vec2D k_default_scale_2d = {100, 100};

namespace descriptors {
inline const auto k_shape2d_descriptor = PropertyDescriptor("Path", PropertyType::e_Shape2D);
inline const auto k_color_descriptor = PropertyDescriptor("Color", PropertyType::e_Color);
inline const auto k_anchor_2d_descriptor = PropertyDescriptor("Anchor", PropertyType::e_Anchor2D);
inline const auto k_scale_2d_descriptor = PropertyDescriptor("Scale", PropertyType::e_Scale2D);
inline const auto k_skew_descriptor = PropertyDescriptor("Skew", PropertyType::e_Skew);
inline const auto k_skew_axis_descriptor = PropertyDescriptor("Skew Axis", PropertyType::e_SkexAxis);
} // namespace descriptors

#define ADD_COLOR ADD_DYNAMIC_PROPERTY(Vec3D, color, descriptors::k_color_descriptor)
#define ADD_PATH ADD_DYNAMIC_PROPERTY(core::BezierPath, path, descriptors::k_shape2d_descriptor)
#define ADD_ANCHOR_2D ADD_DYNAMIC_PROPERTY(Vec2D, anchor, descriptors::k_anchor_2d_descriptor)
#define ADD_SCALE_2D \
    ADD_DYNAMIC_PROPERTY_WITH_DEFAULT_VALUE(Vec2D, \
                                            scale, \
                                            descriptors::k_scale_2d_descriptor, \
                                            k_default_scale_2d)

#define ADD_SKEW ADD_DYNAMIC_PROPERTY(Vec1D, skew, descriptors::k_skew_descriptor)
#define ADD_SKEW_AXIS ADD_DYNAMIC_PROPERTY(Vec1D, skew_axis, descriptors::k_skew_axis_descriptor)

} // namespace inae::model
#endif // SHAPE_PROPERTIES_MACROS_H
