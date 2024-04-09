#include "color_type.h"
#include <fmt/format.h>

namespace inae {
std::string skcolor_to_rgb_string(SkColor color)
{
    return fmt::format("#{:06x}", 0x00FFFFFF & color);
}
} // namespace inae
