#ifndef TEXT_H
#define TEXT_H

#include <core/alive_types/common_types.h>
#include <core/font.h>
#include <core/model/keyframes/keyframe_utility.h>
#include <core/stroke_types.h>

namespace alive::core {
class Text
{
public:
    std::string m_text;
    Font m_font;

    bool m_fill = true;
    Color m_fill_color = {1.0, 1.0, 1.0};

    bool m_stroke = false;
    Vec1D m_stroke_width = 0.0f;
    Color m_stroke_color = {0.0, 0.0, 0.0};

    CapStyle m_cap_style = CapStyle::e_Butt;
    JoinStyle m_join_style = JoinStyle::e_Miter;

    Vec1D m_justify;
    Vec1D m_baseline_shift;
    Vec1D m_line_height;
    bool m_stroke_overfill;

    bool operator!=(const Text &other) const;
    Rect2D bbox() const;

    Text() = default;
    Text(const Text &other);
    Text &operator=(const Text &other);

    Text(Text &&other);
    Text &operator=(Text &&other);

    void set_is_layer_text(bool flag);
    void recalculateBbox();

private:
    Rect2D m_bbox;
    bool m_is_layer_text = true;
};

template<>
inline Text interpolate<Text>(const Text &from, const Text &to, float val)
{
    if (val >= 1.0f) {
        return to;
    }
    return from;
}
} // namespace alive::core
#endif // TEXT_H
