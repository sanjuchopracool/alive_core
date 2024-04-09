#ifndef REPEATER_PROPERTIES_H
#define REPEATER_PROPERTIES_H

#include <core/model/property/property_macros.h>
#include <core/model/property/property_type.h>

namespace inae::model {
namespace descriptors {
inline const auto k_copies_descriptor = PropertyDescriptor("Copies", PropertyType::e_Float);
inline const auto k_offset_descriptor = PropertyDescriptor("Offset", PropertyType::e_Float);
} // namespace descriptors

namespace validator {
inline const auto k_copies_validator = MinMaxPropertyValueValidator<Vec1D>(0);
}

#define ADD_COPIES \
    ADD_DYNAMIC_PROPERTY_WITH_VALIDATOR(Vec1D, \
                                        copies, \
                                        descriptors::k_copies_descriptor, \
                                        3, \
                                        &validator::k_copies_validator)
#define ADD_OFFSET \
    ADD_DYNAMIC_PROPERTY_WITH_DEFAULT_VALUE(Vec1D, offset, descriptors::k_offset_descriptor, 0)

} // namespace inae::model

#endif // REPEATER_PROPERTIES_H
