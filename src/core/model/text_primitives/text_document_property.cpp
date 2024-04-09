#include "text_document_property.h"
#include <core/asset_management/font_manager.h>
#include <core/resource_manager.h>
#include <core/serializer/json_helper.h>
#include <include/core/SkFont.h>
#include <include/core/SkFontMetrics.h>
#include <include/core/SkFontMgr.h>
#include <include/core/SkTextBlob.h>

namespace inae::core {

namespace {
const char k_font_size_key[] = "s";
const char k_fill_color_key[] = "fc";
const char k_font_family_key[] = "f";
const char k_text_key[] = "t";
const char k_stroke_color_key[] = "sc";
const char k_stroke_width_key[] = "sw";
} // namespace

TextDocumentProperty::TextDocumentProperty() {}

bool TextDocumentProperty::operator!=(const TextDocumentProperty &other) const
{
    return (m_font != other.m_font) || (m_fill != other.m_fill) || (m_stroke != other.m_stroke)
           || (m_stroke_width != other.m_stroke_width) || (m_fill_color != other.m_fill_color)
           || (m_stroke_color != other.m_stroke_color) || (m_text != other.m_text)
           || (m_cap_style != other.m_cap_style) || (m_join_style != other.m_join_style);
}

void TextDocumentProperty::decode(json::JsonObject &in_value)
{
    using namespace inae::json;
    auto temp_json = json_pop(in_value, k_font_size_key);
    if (!temp_json.is_null()) {
        m_font.size = temp_json;
    }
    temp_json = json_pop(in_value, k_fill_color_key);
    if (!temp_json.is_null()) {
        json::decode(m_fill_color, temp_json);
    } else {
        m_fill = false;
    }

    temp_json = json_pop(in_value, k_font_family_key);
    if (!temp_json.is_null()) {
        std::string family_name = temp_json;
        int index = FontManager::instance().family_index(family_name);
        if (index == -1) {
            m_font.family = FontManager::instance().default_index().first;
        } else {
            m_font.family = index;
        }
        m_font.style = FontManager::instance().default_index().second;
    }
    temp_json = json_pop(in_value, k_text_key);
    if (!temp_json.is_null()) {
        m_text = temp_json;
    }

    temp_json = json_pop(in_value, k_stroke_color_key);
    if (!temp_json.is_null()) {
        json::decode(m_stroke_color, temp_json);
        m_stroke = true;
    }

    temp_json = json_pop(in_value, k_stroke_width_key);
    if (!temp_json.is_null()) {
        m_stroke_width = temp_json;
    }
}

json::OrderedJsonObject TextDocumentProperty::to_json() const
{
    json::OrderedJsonObject result;
    result[k_font_size_key] = m_font.size;
    result[k_font_family_key] = FontManager::instance().family_name(m_font.family);
    if (m_fill) {
        result[k_fill_color_key] = json::json_value(m_fill_color);
    }
    if (m_stroke) {
        result[k_stroke_color_key] = json::json_value(m_stroke_color);
        result[k_stroke_width_key] = m_stroke_width;
    }
    result[k_text_key] = m_text;
    return result;
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
