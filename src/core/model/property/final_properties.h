#ifndef FINAL_PROPERTIES_CPP
#define FINAL_PROPERTIES_CPP

#include "property_descriptors.h"
#include "property_macros.h"
#include "property_type.h"

namespace inae::model {

inline static Vec3D k_default_transfor_3d = {100, 100, 100};

namespace validators {
inline const PropertyValueValidator<float> *opacity_validator()
{
    static const auto opacity_validator = MinMaxPropertyValueValidator<float>(0.0f, 100.0f);
    return &opacity_validator;
}

inline const PropertyValueValidator<float> *roundness_validator()
{
    static const auto roundness_validator = MinMaxPropertyValueValidator<float>(0.0f);
    return &roundness_validator;
}

inline const PropertyValueValidator<float> *width_validator()
{
    static const auto width_validator = MinMaxPropertyValueValidator<float>(0.0f);
    return &width_validator;
}

inline const PropertyValueValidator<int> *min_points_validator()
{
    static const auto min_points_validator = MinMaxPropertyValueValidator<int>(3);
    return &min_points_validator;
}

inline const PropertyValueValidator<Vec2D> *size_2d_validator()
{
    static const auto size_2d_validator = Vec2DMinPropertyValueValidator<Vec2D>(Vec2D{0, 0});
    return &size_2d_validator;
}

} // namespace validators

#define ADD_ANCHOR_3D ADD_DYNAMIC_PROPERTY(Vec3D, anchor, inae::descriptors::k_anchor_3d_descriptor)

#define ADD_POSITION_3D \
    ADD_DYNAMIC_PROPERTY(Vec3D, position, inae::descriptors::k_position_3d_descriptor)
#define ADD_SIZE_2D \
    ADD_DYNAMIC_PROPERTY_WITH_VALIDATOR(Vec2D, \
                                        size, \
                                        inae::descriptors::k_size_2d_descriptor, \
                                        (Vec2D{1, 1}), \
                                        validators::size_2d_validator())

#define ADD_POSITION_2D \
    ADD_DYNAMIC_PROPERTY(Vec2D, position, inae::descriptors::k_position_2d_descriptor)

#define ADD_SCALE_3D \
    ADD_DYNAMIC_PROPERTY_WITH_DEFAULT_VALUE(Vec3D, \
                                            scale, \
                                            inae::descriptors::k_scale_3d_descriptor, \
                                            k_default_transfor_3d)
#define ADD_ROTATION \
    ADD_DYNAMIC_PROPERTY_WITH_DEFAULT_VALUE(float, \
                                            rotation, \
                                            inae::descriptors::k_rotation_descriptor, \
                                            0.0f)
#define ADD_ROUNDNESS \
    ADD_DYNAMIC_PROPERTY_WITH_VALIDATOR(float, \
                                        roundness, \
                                        inae::descriptors::k_roundness_descriptor, \
                                        0.0f, \
                                        validators::roundness_validator())
#define ADD_WIDTH \
    ADD_DYNAMIC_PROPERTY_WITH_VALIDATOR(float, \
                                        width, \
                                        inae::descriptors::k_width_descriptor, \
                                        1.0f, \
                                        validators::width_validator())
#define ADD_OPACITY \
    ADD_DYNAMIC_PROPERTY_WITH_VALIDATOR(float, \
                                        opacity, \
                                        inae::descriptors::k_opacity_descriptor, \
                                        100.0f, \
                                        validators::opacity_validator())

#define ADD_POINTS \
    ADD_DYNAMIC_PROPERTY_WITH_VALIDATOR(int, \
                                        points, \
                                        inae::descriptors::k_num_points_descriptor, \
                                        3, \
                                        validators::min_points_validator())

} // namespace inae::model
#endif // FINAL_PROPERTIES_CPP
