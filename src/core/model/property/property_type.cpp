#include "property_type.h"
#include "property_type_information.h"
#include <core/alive_types/common_types.h>
#include <map>

namespace alive {
namespace model {

static const std::map<PropertyType, PropertyTypeInformation> k_type_to_info{
    {PropertyType::e_StaticSingleLineText, {"String (one line)", "Text", "One line text"}},
    {PropertyType::e_StaticColor3V, {"Color (RGB)", "Color", "Color in RGB format"}},
    {PropertyType::e_StaticColor4ub, {"Color (RGBA)", "Color", "Color in RGBA format"}},
    {PropertyType::e_StaticInt, {"Int", "Value", "Integer Values"}},
    {PropertyType::e_StaticPositveInt, {"Positive Int", "Value", "Psoitive integer values"}},
    {PropertyType::e_StaticFloat, {"Real", "Value", "Real value"}},
    {PropertyType::e_StaticPositiveFloat, {"Positive Real", "Value", "Positive real value"}},
    {PropertyType::e_StaticSizeFactor, {"Factor", "Factor", "Value between 0.0 and 1.0"}},
    {PropertyType::e_Font, {"Font", "Font", "Font family, style and size"}},
    {PropertyType::e_Direction, {"Direction", "Direction", "Left, Right, Top or Bottom"}},
    {PropertyType::e_Bool, {"Boolean", "Flag", "True or false"}}};

const PropertyTypeInformation k_unknown = {"Unknown", ""};

const PropertyTypeInformation &info_from_property_type(PropertyType type)
{
    // assertm(k_type_to_info.size()
    //             == (static_cast<int>(PropertyType::e_StaticPropertyEnd)
    //                 - static_cast<int>(PropertyType::e_StaticPropertyStart)),
    //         "Add information for more properties!");
    auto it = k_type_to_info.find(type);
    if (it != k_type_to_info.end()) {
        return it->second;
    }
    return k_unknown;
}
} // namespace model
} // namespace alive
