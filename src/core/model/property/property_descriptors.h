#ifndef PROPERTY_DESCRIPTORS_H
#define PROPERTY_DESCRIPTORS_H

#include "property_descriptor.h"
#include "property_type.h"

namespace alive::descriptors {
using namespace alive::model;
inline const auto k_anchor_3d_descriptor = PropertyDescriptor("Anchor Point",
                                                              PropertyType::e_Anchor3D);
inline const auto k_position_3d_descriptor = PropertyDescriptor("Position",
                                                                PropertyType::e_Position3D);
inline const auto k_position_2d_descriptor = PropertyDescriptor("Position",
                                                                PropertyType::e_Position2D);
inline const auto k_size_2d_descriptor = PropertyDescriptor("Size", PropertyType::e_Size2D);
inline const auto k_scale_3d_descriptor = PropertyDescriptor("Scale",
                                                             PropertyType::e_Scale3D,
                                                             PropertyDescriptor::e_Linked);
inline const auto k_rotation_descriptor = PropertyDescriptor("Rotation", PropertyType::e_Rotation);
inline const auto k_roundness_descriptor = PropertyDescriptor("Roundness", PropertyType::e_Float);
inline const auto k_width_descriptor = PropertyDescriptor("Width", PropertyType::e_Float);
inline const auto k_opacity_descriptor = PropertyDescriptor("Opacity", PropertyType::e_Opacity);
inline const auto k_num_points_descriptor = PropertyDescriptor("Points", PropertyType::e_Float);

inline const auto k_skew_descriptor = PropertyDescriptor("Skew", PropertyType::e_Rotation);
inline const auto k_skew_axis_descriptor = PropertyDescriptor("Skew Axis", PropertyType::e_Rotation);

} // namespace alive::descriptors
#endif // PROPERTY_DESCRIPTORS_H
