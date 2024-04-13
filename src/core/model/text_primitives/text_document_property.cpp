#include "text_document_property.h"
#include <core/asset_management/font_manager.h>
#include <core/resource_manager.h>
#include <core/serializer/json_helper.h>
#include <include/core/SkFont.h>
#include <include/core/SkFontMetrics.h>
#include <include/core/SkFontMgr.h>
#include <include/core/SkTextBlob.h>

namespace inae::core {

TextDocumentProperty::TextDocumentProperty() {}

bool TextDocumentProperty::operator!=(const TextDocumentProperty &other) const
{
    return (m_font != other.m_font) || (m_fill != other.m_fill) || (m_stroke != other.m_stroke)
           || (m_stroke_width != other.m_stroke_width) || (m_fill_color != other.m_fill_color)
           || (m_stroke_color != other.m_stroke_color) || (m_text != other.m_text)
           || (m_cap_style != other.m_cap_style) || (m_join_style != other.m_join_style);
}

SkRect TextDocumentProperty::bbox() const
{
    SkRect rect;
    SkFontStyle font_style;
    const auto &font_mgr = core::FontManager::instance();
    sk_sp<SkTypeface> typeface;
    SkTypeface *font_impl = font_mgr.font_impl(m_font.family, m_font.style);
    typeface.reset(font_impl);
    if (typeface) {
        SkFont font(typeface, m_font.size);
        SkPaint paint;
        if (m_stroke) {
            paint.setStyle(SkPaint::kStroke_Style);
            paint.setAntiAlias(true);
            paint.setStrokeWidth(m_stroke_width);
        }
        font.measureText(m_text.c_str(), m_text.size(), SkTextEncoding::kUTF8, &rect, &paint);
        rect = rect.makeOffset(-rect.width() / 2, 0);
        font_impl = typeface.release();
    }
    return rect;
}

} // namespace inae::core
