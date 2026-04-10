#include "color_type.h"
#include <format>
#include <sol/sol.hpp>

namespace alive {
std::string color_to_rgb_string(Color4ub color)
{
    uint32_t clr = (color.r() << 16) | (color.g() << 8) | color.b();
    return std::format("#{:06x}", 0x00FFFFFF & clr);
}

std::string color_to_rgba_string(Color4ub color)
{
    uint32_t clr = (color.r() << 24) | (color.g() << 16) | (color.b() << 8) | color.a();
    return std::format("#{:08x}", 0xFFFFFFFF & clr);
}

Color color_4ub_to_3v(Color4ub color)
{
    return Color(color.r() / 255.0, color.g() / 255.0, color.b() / 255.0);
}

Vec1D color_4ub_alpha_percentage(Color4ub color)
{
    return color.a() / 255.0 * 100.0;
}

void register_color_types(sol::state &lua)
{
    /*------------------------------------------------------------------------------------
    // Color  
    ------------------------------------------------------------------------------------*/
    {
        sol::usertype<Color> color_3v_type = lua.new_usertype<Color>("Color");
        color_3v_type.set("r",
                          sol::property([](Color &type) { return type.r(); },
                                        [](Color &type, Vec1D val) { type.r() = val; }));
        color_3v_type.set("g",
                          sol::property([](Color &type) { return type.g(); },
                                        [](Color &type, Vec1D val) { type.g() = val; }));
        color_3v_type.set("b",
                          sol::property([](Color &type) { return type.b(); },
                                        [](Color &type, Vec1D val) { type.b() = val; }));
    }

    /*------------------------------------------------------------------------------------
    // Color  4ub
    ------------------------------------------------------------------------------------*/
    {
        sol::usertype<Color4ub> color_4ub_type = lua.new_usertype<Color4ub>("Color4ub");
        color_4ub_type.set("r",
                           sol::property([](Color4ub &type) { return type.r(); },
                                         [](Color4ub &type, uint8_t val) { type.r() = val; }));
        color_4ub_type.set("g",
                           sol::property([](Color4ub &type) { return type.g(); },
                                         [](Color4ub &type, uint8_t val) { type.g() = val; }));
        color_4ub_type.set("b",
                           sol::property([](Color4ub &type) { return type.b(); },
                                         [](Color4ub &type, uint8_t val) { type.b() = val; }));
        color_4ub_type.set("a",
                           sol::property([](Color4ub &type) { return type.a(); },
                                         [](Color4ub &type, uint8_t val) { type.a() = val; }));
        color_4ub_type.set("color", sol::readonly_property([](const Color4ub &type) {
                               return color_4ub_to_3v(type);
                           }));
        color_4ub_type.set("alpha_percentage", sol::readonly_property([](const Color4ub &type) {
                               return color_4ub_alpha_percentage(type);
                           }));
    }

    lua["color4ub_to_color3v"] = color_4ub_to_3v;
    lua["color4ub_alpha_percentage"] = color_4ub_alpha_percentage;
}
} // namespace alive
