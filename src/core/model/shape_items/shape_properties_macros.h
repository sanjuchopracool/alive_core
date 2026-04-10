#ifndef SHAPE_PROPERTIES_MACROS_H
#define SHAPE_PROPERTIES_MACROS_H

#include <core/alive_types/color_type.h>
#include <core/model/property/final_properties.h>
#include <core/model/property/property_descriptor.h>
#include <core/model/property/property_macros.h>
#include <core/model/property/property_type.h>
#include <core/path_primitives/bezier_path.h>
namespace alive::model {

constexpr Vec2D k_default_scale_2d = {100, 100};

namespace descriptors {
inline const auto k_shape2d_descriptor = PropertyDescriptor("Path", PropertyType::e_Shape2D);
inline const auto k_color_descriptor = PropertyDescriptor("Color", PropertyType::e_Color);
inline const auto k_anchor_2d_descriptor = PropertyDescriptor("Anchor", PropertyType::e_Anchor2D);
inline const auto k_scale_2d_descriptor = PropertyDescriptor("Scale",
                                                             PropertyType::e_Scale2D,
                                                             PropertyDescriptor::e_Linked);
inline const auto k_inner_radius_descriptor = PropertyDescriptor("Inner Radius",
                                                                 PropertyType::e_Float);
inline const auto k_outer_radius_descriptor = PropertyDescriptor("Outer Radius",
                                                                 PropertyType::e_Float);
inline const auto k_inner_roundness_descriptor = PropertyDescriptor("Inner Roundness",
                                                                    PropertyType::e_Roundness);
inline const auto k_outer_roundness_descriptor = PropertyDescriptor("Outer Roundness",
                                                                    PropertyType::e_Roundness);

inline const auto k_start_position_descriptor = PropertyDescriptor("Start Position",
                                                                   PropertyType::e_Position2D);
inline const auto k_end_position_descriptor = PropertyDescriptor("End Position",
                                                                 PropertyType::e_Position2D);
} // namespace descriptors

#define ADD_COLOR ADD_DYNAMIC_PROPERTY(Color, color, descriptors::k_color_descriptor)
#define ADD_PATH ADD_DYNAMIC_PROPERTY(core::BezierPath, path, descriptors::k_shape2d_descriptor)
#define ADD_ANCHOR_2D ADD_DYNAMIC_PROPERTY(Vec2D, anchor, descriptors::k_anchor_2d_descriptor)
#define ADD_SCALE_2D \
    ADD_LINKED_DYNAMIC_PROPERTY_WITH_DEFAULT_VALUE(Vec2D, \
                                                   scale, \
                                                   descriptors::k_scale_2d_descriptor, \
                                                   k_default_scale_2d)

#define ADD_INNER_RADIUS \
    ADD_DYNAMIC_PROPERTY_WITH_VALIDATOR(Vec1D, \
                                        inner_radius, \
                                        descriptors::k_inner_radius_descriptor, \
                                        0.0f, \
                                        validators::roundness_validator())
#define ADD_OUTER_RADIUS \
    ADD_DYNAMIC_PROPERTY_WITH_VALIDATOR(Vec1D, \
                                        outer_radius, \
                                        descriptors::k_outer_radius_descriptor, \
                                        0.0f, \
                                        validators::roundness_validator())

#define ADD_INNER_ROUNDNESS \
    ADD_DYNAMIC_PROPERTY_WITH_DEFAULT_VALUE(Vec1D, \
                                            inner_roundness, \
                                            descriptors::k_inner_roundness_descriptor, \
                                            0.0f)
#define ADD_OUTER_ROUNDNESS \
    ADD_DYNAMIC_PROPERTY_WITH_DEFAULT_VALUE(Vec1D, \
                                            outer_roundness, \
                                            descriptors::k_outer_roundness_descriptor, \
                                            0.0f)

} // namespace alive::model
#endif // SHAPE_PROPERTIES_MACROS_H
