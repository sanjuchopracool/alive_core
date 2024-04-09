#ifndef TEXTDOCUMENTPROPERTY_H
#define TEXTDOCUMENTPROPERTY_H

#include <core/common_types.h>
#include <core/font.h>
#include <core/model/keyframes/keyframe_utility.h>
#include <core/private/json.h>
#include <include/core/SkPaint.h>

namespace inae::json {
using JsonObject = nlohmann::json;
}

namespace inae::core {
class TextDocumentProperty
{
public:
    TextDocumentProperty();
    Font m_font;

    Vec3D m_fill_color = {1.0, 1.0, 1.0};
    bool m_fill = true;
    std::string m_text;
    float m_justify;
    float m_baseline_shift;
    float m_line_height;
    float m_stroke_width = 0.0f;
    Vec3D m_stroke_color = {0.0, 0.0, 0.0};
    bool m_stroke = false;
    bool m_stroke_overfill;
    SkPaint::Cap m_cap_style = SkPaint::kButt_Cap;
    SkPaint::Join m_join_style = SkPaint::kMiter_Join;

    bool operator!=(const TextDocumentProperty &other) const;
    void decode(json::JsonObject &in_value);
    json::OrderedJsonObject to_json() const;

    SkRect bbox() const;
};

template<>
inline TextDocumentProperty interpolate<TextDocumentProperty>(const TextDocumentProperty &from,
                                                              const TextDocumentProperty &to,
                                                              float val)
{
    if (val >= 1.0f) {
        return to;
    }
    return from;
}
} // namespace inae::core
#endif // TEXTDOCUMENTPROPERTY_H
