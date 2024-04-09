#ifndef PROPERTYTYPE_H
#define PROPERTYTYPE_H

namespace inae::model {

enum class PropertyType {
    e_Unknown,
    e_Object,
    e_Bool,
    e_Enum,

    e_AnimatiedTypeStart,
    e_Int = e_AnimatiedTypeStart,
    e_Float,
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
    e_OuterRadius,
    e_InnerRadius,
    e_OuterRoundness,
    e_InnerRoundness,
    e_NumPoints,
    e_Shape2D,
    e_Skew,
    e_SkexAxis,
    e_PropertyLast,
    e_TextDocument,
};

} // namespace inae::model

#endif // PROPERTYTYPE_H
