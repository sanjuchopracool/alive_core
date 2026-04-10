#ifndef TRIM_PROPERTIES_H
#define TRIM_PROPERTIES_H

#include "property_macros.h"
#include <core/model/property/property_type.h>

namespace alive::model {
namespace descriptors {
inline const auto k_trim_start_descriptor = PropertyDescriptor("Start", PropertyType::e_Float);
inline const auto k_trim_end_descriptor = PropertyDescriptor("End", PropertyType::e_Float);
inline const auto k_trim_offset_descriptor = PropertyDescriptor("Offset", PropertyType::e_Rotation);
} // namespace descriptors

namespace validators {
inline const auto k_trim_start_validator = MinMaxPropertyValueValidator<float>(0.0f, 100.0f);
inline const auto k_trim_end_validator = MinMaxPropertyValueValidator<float>(0.0f, 100.0f);
} // namespace validators

#define ADD_TRIM_START \
    ADD_DYNAMIC_PROPERTY_WITH_VALIDATOR(float, \
                                        start, \
                                        descriptors::k_trim_start_descriptor, \
                                        0.0f, \
                                        &validators::k_trim_start_validator)
#define ADD_TRIM_END \
    ADD_DYNAMIC_PROPERTY_WITH_VALIDATOR(float, \
                                        end, \
                                        descriptors::k_trim_end_descriptor, \
                                        100.0f, \
                                        &validators::k_trim_end_validator)
#define ADD_TRIM_OFFSET ADD_DYNAMIC_PROPERTY(float, offset, descriptors::k_trim_offset_descriptor)
} // namespace alive::model

#endif // TRIM_PROPERTIES_H
