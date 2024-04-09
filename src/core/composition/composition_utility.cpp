#include "composition_utility.h"
#include <include/core/SkBlendMode.h>
#include <core/model/layers/layer.h>

namespace inae::model {
std::optional<SkBlendMode> to_sk_blend_mode(BlendMode in_mode)
{
    std::optional<SkBlendMode> mode;
    if (in_mode != BlendMode::e_Normal) {
        switch (in_mode) {
        case BlendMode::e_Multiply:
            mode = SkBlendMode::kMultiply;
            break;
        case BlendMode::e_Screen:
            mode = SkBlendMode::kScreen;
            break;
        case BlendMode::e_Overlay:
            mode = SkBlendMode::kOverlay;
            break;
        case BlendMode::e_Darken:
            mode = SkBlendMode::kDarken;
            break;
        case BlendMode::e_Lighten:
            mode = SkBlendMode::kLighten;
            break;
        case BlendMode::e_ColorDodge:
            mode = SkBlendMode::kColorDodge;
            break;
        case BlendMode::e_ColorBurn:
            mode = SkBlendMode::kColorBurn;
            break;
        case BlendMode::e_HardLight:
            mode = SkBlendMode::kHardLight;
            break;
        case BlendMode::e_SoftLight:
            mode = SkBlendMode::kSoftLight;
            break;
        case BlendMode::e_Difference:
            mode = SkBlendMode::kDifference;
            break;
        case BlendMode::e_Exclusion:
            mode = SkBlendMode::kExclusion;
            break;
        case BlendMode::e_Hue:
            mode = SkBlendMode::kHue;
            break;
        case BlendMode::e_Saturation:
            mode = SkBlendMode::kSaturation;
            break;
        case BlendMode::e_Color:
            mode = SkBlendMode::kColor;
            break;
        case BlendMode::e_Luminosity:
            mode = SkBlendMode::kLuminosity;
            break;
        case BlendMode::e_Add:
            mode = SkBlendMode::kPlus;
            break;
        default:
            break;
        }
    }
    return mode;
}
} // namespace inae::model
