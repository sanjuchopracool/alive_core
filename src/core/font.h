#ifndef FONT_H
#define FONT_H

namespace alive::core {
struct Font
{
    int family = 0;
    int style = 0;
    float size = 10.0f;

    bool operator==(const Font &other) const
    {
        return family == other.family && style == other.style && size == other.size;
    }
};
} // namespace alive::core

#endif // FONT_H
