#ifndef FONT_H
#define FONT_H

namespace inae::core {
struct Font
{
    float size = 10.0f;
    int family = 0;
    int style = 0;

    bool operator==(const Font &other) const
    {
        return size == other.size && family == other.family && style == other.style;
    }
};
} // namespace inae::core

#endif // FONT_H
