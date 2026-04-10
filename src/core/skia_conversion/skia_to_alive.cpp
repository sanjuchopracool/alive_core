#include "skia_to_alive.h"
#include <core/logging/logger.h>
#include <include/core/SkFontStyle.h>
#include <map>

namespace alive::skia {

namespace {
struct CompareSkFontStyle
{
    bool operator()(const SkFontStyle &l, const SkFontStyle &r) const
    {
        if (l.width() < r.width()) {
            return true;
        }
        if (l.width() > r.width()) {
            return false;
        }

        if (l.weight() < r.weight()) {
            return true;
        }

        if (l.weight() > r.weight()) {
            return false;
        }

        if (l.slant() < r.slant()) {
            return true;
        }

        if (l.slant() > r.slant()) {
            return false;
        }

        return false;
    }
};

static std::map<SkFontStyle, std::string, CompareSkFontStyle> font_style_to_name;
} // namespace

std::string font_weight_to_string(int weight)
{
    std::string wight_style;
    ALIVE_CORE_TRACE("FONT_WEIGHT: {}", weight);
    switch (weight) {
    case SkFontStyle::kInvisible_Weight:
        wight_style += "Invisible";
        break;
    case SkFontStyle::kThin_Weight:
        wight_style += "Thin";
        break;
    case SkFontStyle::kExtraLight_Weight:
        wight_style += "ExtraLight";
        break;
    case SkFontStyle::kLight_Weight:
        wight_style += "Light";
        break;
    case SkFontStyle::kNormal_Weight:
        wight_style += "Regular";
        break;
    case SkFontStyle::kMedium_Weight:
        wight_style += "Medium";
        break;
    case SkFontStyle::kSemiBold_Weight:
        wight_style += "SemiBold";
        break;
    case SkFontStyle::kBold_Weight:
        wight_style += "Bold";
        break;
    case SkFontStyle::kExtraBold_Weight:
        wight_style += "ExtraBold";
        break;
    case SkFontStyle::kBlack_Weight:
        wight_style += "Black";
        break;
    case SkFontStyle::kExtraBlack_Weight:
        wight_style += "ExtraBlack";
        break;
    default:
        break;
    }
    if (wight_style.empty()) {
        if (weight > SkFontStyle::kExtraBlack_Weight) {
            wight_style = "ExtraBlack+";
        } else if (weight > SkFontStyle::kBlack_Weight) {
            wight_style = "Black+";
        } else if (weight > SkFontStyle::kExtraBold_Weight) {
            wight_style = "ExtraBold+";
        } else if (weight > SkFontStyle::kBold_Weight) {
            wight_style = "Bold+";
        } else if (weight > SkFontStyle::kSemiBold_Weight) {
            wight_style = "SemiBold+";
        } else if (weight > SkFontStyle::kMedium_Weight) {
            wight_style = "Medium+";
        } else if (weight > SkFontStyle::kNormal_Weight) {
            wight_style = "Normal+";
        } else if (weight > SkFontStyle::kLight_Weight) {
            wight_style = "Light+";
        } else if (weight > SkFontStyle::kExtraLight_Weight) {
            wight_style = "ExtraLight+";
        } else if (weight > SkFontStyle::kThin_Weight) {
            wight_style = "Thin+";
        } else if (weight > SkFontStyle::kInvisible_Weight) {
            wight_style = "Invisible+";
        }
    }
    return wight_style;
}
std::string font_width_to_string(int width)
{
    std::string width_style;
    ALIVE_CORE_TRACE("FONT_WIDTH: {}", width);
    switch (width) {
    case SkFontStyle::kUltraCondensed_Width:
        width_style += "UltraCondensed";
        break;
    case SkFontStyle::kExtraCondensed_Width:
        width_style += "ExtraCondensed";
        break;
    case SkFontStyle::kCondensed_Width:
        width_style += "Condensed";
        break;
    case SkFontStyle::kSemiCondensed_Width:
        width_style += "SemiCondensed";
        break;
    case SkFontStyle::kNormal_Width:
        // width_style += "Regular";
        break;
    case SkFontStyle::kSemiExpanded_Width:
        width_style += "SemiExpanded";
        break;
    case SkFontStyle::kExpanded_Width:
        width_style += "Expanded";
        break;
    case SkFontStyle::kExtraExpanded_Width:
        width_style += "ExtraExpanded";
        break;
    case SkFontStyle::kUltraExpanded_Width:
        width_style += "UltraExpanded";
        break;
    default:
        break;
    }
    return width_style;
}

std::string font_slant_to_string(int slant)
{
    std::string width_style;
    ALIVE_CORE_TRACE("FONT_SLANT: {}", slant);
    switch (slant) {
    case SkFontStyle::kUpright_Slant:
        // width_style += "Upright";
        break;
    case SkFontStyle::kItalic_Slant:
        width_style += "Italic";
        break;
    case SkFontStyle::kOblique_Slant:
        width_style += "Oblique";
        break;
    default:
        break;
    }
    return width_style;
}

std::string to_string(const SkFontStyle &style)
{
    auto it = font_style_to_name.find(style);
    if (it != font_style_to_name.end()) {
        return it->second;
    }

    std::string result = font_weight_to_string(style.weight()) + font_width_to_string(style.width())
                         + font_slant_to_string(style.slant());
    font_style_to_name[style] = result;
    return result;
}

SkMatrix to_skmatrix(const Matrix3D &mat)
{
    auto scales = mat.scaling();
    return SkMatrix::Scale(scales.x(), scales.y());
}

} // namespace alive::skia
