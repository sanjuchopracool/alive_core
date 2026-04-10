#include "text.h"
#include <core/asset_management/font_manager.h>
#include <core/logging/logger.h>
#include <core/model/text_primitives/singleline_run_handler.h>
#include <core/resource_manager.h>
#include <core/serializer/json_helper.h>
#include <include/core/SkFont.h>
#include <include/core/SkFontMetrics.h>
#include <include/core/SkFontMgr.h>
#include <include/core/SkPaint.h>
#include <include/core/SkTextBlob.h>
#include <modules/skshaper/include/SkShaper.h>
#include <modules/skshaper/include/SkShaper_harfbuzz.h>
#include <modules/skunicode/include/SkUnicode.h>
#include <modules/skunicode/include/SkUnicode_icu.h>
#include <third_party/externals/harfbuzz/src/hb-aat.h>
#include <third_party/externals/harfbuzz/src/hb-ot.h>
#include <third_party/externals/harfbuzz/src/hb.h>

namespace alive::core {
sk_sp<SkUnicode> get_unicode()
{
    auto unicode = SkUnicodes::ICU::Make();
    if (unicode) {
        return unicode;
    } else {
        ALIVE_CORE_ERROR("Invalid unicode,  not available");
        return nullptr;
    }
}

bool Text::operator!=(const Text &other) const
{
    return (m_font != other.m_font) || (m_fill != other.m_fill) || (m_stroke != other.m_stroke)
           || (m_stroke_width != other.m_stroke_width) || (m_fill_color != other.m_fill_color)
           || (m_stroke_color != other.m_stroke_color) || (m_text != other.m_text)
           || (m_cap_style != other.m_cap_style) || (m_join_style != other.m_join_style);
}

Rect2D Text::bbox() const
{
    return m_bbox;
}

Text::Text(const Text &other)
{
    *this = other;
}

Text &Text::operator=(const Text &other)
{
    if (this != &other) {
        m_text = other.m_text;
        m_font = other.m_font;

        m_fill = other.m_fill;
        m_fill_color = other.m_fill_color;

        m_stroke = other.m_stroke;
        m_stroke_width = other.m_stroke_width;
        m_stroke_color = other.m_stroke_color;

        m_cap_style = other.m_cap_style;
        m_join_style = other.m_join_style;

        m_justify = other.m_justify;
        m_baseline_shift = other.m_baseline_shift;
        m_line_height = other.m_line_height;
        m_stroke_overfill = other.m_stroke_overfill;
        recalculateBbox();
    }
    return *this;
}

void Text::set_is_layer_text(bool flag)
{
    if (flag && m_is_layer_text == false) {
        m_is_layer_text = true;
        recalculateBbox();
    }
    m_is_layer_text = flag;
}

Text::Text(Text &&other)
{
    *this = other;
}

Text &Text::operator=(Text &&other)
{
    *this = other;
    return *this;
}

void Text::recalculateBbox()
{
    if (m_is_layer_text) {
        std::unique_ptr<SkShaper> shaper = SkShapers::HB::ShaperDrivenWrapper(get_unicode(),
                                                                              nullptr);
        if (!shaper) {
            ALIVE_CORE_ERROR("Text Shaper not available");
            return;
        }

        const auto &font_mgr = core::FontManager::instance();
        sk_sp<SkTypeface> typeface;
        SkTypeface *font_impl = font_mgr.font_impl(m_font.family, m_font.style);
        typeface.reset(font_impl);
        if (!typeface) {
            ALIVE_CORE_ERROR("Text Typeface not available");
            return;
        }

        SkFont font(typeface, m_font.size);
        const char *text = m_text.c_str();
        size_t textBytes = m_text.size();

        SingleLineRunHandler textBlobBuilder(text, {0, 0});

        const SkBidiIterator::Level defaultLevel = SkBidiIterator::kLTR;
        std::unique_ptr<SkShaper::BiDiRunIterator> bidi
            = SkShaper::MakeBiDiRunIterator(text, textBytes, defaultLevel);
        SkASSERT(bidi);

        std::unique_ptr<SkShaper::LanguageRunIterator> language
            = SkShaper::MakeStdLanguageRunIterator(text, textBytes);
        SkASSERT(language);

        std::unique_ptr<SkShaper::ScriptRunIterator> script
            = SkShapers::HB::ScriptRunIterator(text, textBytes);
        SkASSERT(script);

        std::unique_ptr<SkShaper::FontRunIterator> fontRuns
            = SkShaper::MakeFontMgrRunIterator(text, textBytes, font, SkFontMgr::RefEmpty());
        SkASSERT(fontRuns);
        shaper->shape(text,
                      textBytes,
                      *fontRuns,
                      *bidi,
                      *script,
                      *language,
                      nullptr,
                      0,
                      std::numeric_limits<float>::max(),
                      &textBlobBuilder);
        const auto &rect = textBlobBuilder.bbox();
        m_bbox = Rect2D{{rect.left(), rect.top()}, {rect.right(), rect.bottom()}};
        font_impl = typeface.release();
    }
}

} // namespace alive::core
