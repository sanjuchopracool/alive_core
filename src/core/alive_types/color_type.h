#ifndef COLOR_TYPE_H
#define COLOR_TYPE_H

#include "common_types.h"
// #include <include/core/SkColor.h>

namespace sol {
class state;
}

namespace alive {

/**
 * @brief The Color class is wrapper over skcolor adapted to alive
 */

std::string color_to_rgb_string(Color4ub color);
std::string color_to_rgba_string(Color4ub color);
Color color_4ub_to_3v(Color4ub color);
Vec1D color_4ub_alpha_percentage(Color4ub color);

inline Color4ub color_from_rgb_string(const std::string &str)
{
    uint32_t color = static_cast<uint32_t>(std::stoll("0x" + str.substr(1), nullptr, 16));
    uint8_t r = (0xFF0000 & color) >> 16;
    uint8_t g = (0x00FF00 & color) >> 8;
    uint8_t b = (0x0000FF & color);
    return Color4ub{r, g, b, 0xFF};
}

void register_color_types(sol::state &lua);
} // namespace alive
#endif // COLOR_TYPE_H
