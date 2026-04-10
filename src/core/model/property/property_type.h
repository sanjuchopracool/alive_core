#ifndef PROPERTYTYPE_H
#define PROPERTYTYPE_H

namespace alive::model {

enum class PropertyType {
    e_Unknown,

    e_AnimatiedTypeStart = 1000,
    e_Int = e_AnimatiedTypeStart,
    e_Float,
    e_Roundness,
    e_Position2D,
    e_Size2D,
    e_Anchor2D,
    e_Anchor3D,
    e_Position3D,
    e_Scale2D,
    e_Scale3D,
    e_Rotation,
    e_Opacity,
    e_Color,
    e_NumPoints,
    e_Shape2D,
    e_PropertyLast,
    e_TextDocument,
    e_AnimatiedTypeEnd,

    e_StaticPropertyStart = 10000,
    e_StaticSingleLineText = e_StaticPropertyStart,
    e_StaticColor3V,
    e_StaticColor4ub,
    e_StaticInt,
    e_StaticPositveInt,
    e_StaticFloat,
    e_StaticPositiveFloat,
    e_StaticSizeFactor, // float value from 0.0,1.0
    e_Font,
    e_Bool,
    e_Enum,
    e_Direction,
    e_StaticPropertyEnd
};

class PropertyTypeInformation;
const PropertyTypeInformation &info_from_property_type(PropertyType type);
} // namespace alive::model

#endif // PROPERTYTYPE_H
