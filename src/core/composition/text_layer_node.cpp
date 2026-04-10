#include "text_layer_node.h"
#include <core/alive_types/color_type.h>
#include <core/asset_management/font_manager.h>
#include <core/composition/paint_context.h>
#include <core/model/layers/text_layer.h>
#include <core/model/property/dynamic_property_animator.h>
#include <core/model/text_primitives/singleline_run_handler.h>
#include <core/skia_conversion/skia_to_alive.h>
#include <include/core/SkCanvas.h>
#include <include/core/SkFont.h>
#include <include/core/SkFontMetrics.h>
#include <include/core/SkFontMgr.h>
#include <include/core/SkRefCnt.h>
#include <include/core/SkTextBlob.h>
#include <include/core/SkTypeface.h>
#include <modules/skshaper/include/SkShaper.h>
#include <modules/skshaper/include/SkShaper_harfbuzz.h>
#include <modules/skunicode/include/SkUnicode.h>
#include <modules/skunicode/include/SkUnicode_icu.h>
#include <third_party/externals/harfbuzz/src/hb-aat.h>
#include <third_party/externals/harfbuzz/src/hb-ot.h>
#include <third_party/externals/harfbuzz/src/hb.h>

namespace alive::model {

sk_sp<SkUnicode> get_unicode()
{
    auto unicode = SkUnicodes::ICU::Make();
    if (unicode) {
        return unicode;
    } else {
        ALIVE_CORE_ERROR("Unicode not available!!");
        return nullptr;
    }
}

TextLayerNode::TextLayerNode(TextLayer *text_layer)
    : LayerNode(text_layer)
    , m_text_layer(text_layer)
    , m_text(create_animator(text_layer->text(), this))
{}

void TextLayerNode::draw_layer(PaintContext &context) const
{
    const core::Text &t_value = m_text->value();
    if (t_value.m_fill || t_value.m_stroke) {
        const auto &doc_font = t_value.m_font;
        std::unique_ptr<SkShaper> shaper = SkShapers::HB::ShaperDrivenWrapper(get_unicode(),
                                                                              nullptr);
        if (!shaper) {
            ALIVE_CORE_ERROR("Shaper not available");
            return;
        }
        const auto &font_mgr = core::FontManager::instance();
        sk_sp<SkTypeface> typeface;
        SkTypeface *font_impl = font_mgr.font_impl(doc_font.family, doc_font.style);
        typeface.reset(font_impl);
        if (typeface) {
            context.save();
            // create blob here
            {
                SkFont font(typeface, t_value.m_font.size);
                const char *text = t_value.m_text.c_str();
                size_t textBytes = t_value.m_text.size();

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

                const auto &blob = textBlobBuilder.makeBlob();
                SkFontMetrics fm;
                font.getMetrics(&fm);

                auto x = 0;
                auto y = fm.fAscent;
                font.setEdging(SkFont::Edging::kAntiAlias);
                if (t_value.m_fill) {
                    SkPaint fill_paint;
                    fill_paint.setAntiAlias(true);
                    SkColor fill_color = skia::skcolor_from_vec3d(t_value.m_fill_color);
                    int fill_alpha = (context.parent_alpha * SkColorGetA(fill_color) * opacity())
                                     / (255 * 100);
                    fill_color = SkColorSetA(fill_color, fill_alpha);
                    fill_paint.setColor(fill_color);
                    if (context.blend_mode) {
                        fill_paint.setBlendMode(context.blend_mode.value());
                    }
                    context.canvas.drawTextBlob(blob.get(), x, y, fill_paint);
                }

                if (t_value.m_stroke) {
                    SkPaint stroke_paint;
                    stroke_paint.setStyle(SkPaint::kStroke_Style);
                    stroke_paint.setAntiAlias(true);
                    stroke_paint.setStrokeCap(alive::skia::to_sk_cap_style(t_value.m_cap_style));
                    stroke_paint.setStrokeJoin(alive::skia::to_sk_join_style(t_value.m_join_style));
                    auto stroke_color = skia::skcolor_from_vec3d(t_value.m_stroke_color);
                    int fill_alpha = (context.parent_alpha * SkColorGetA(stroke_color) * opacity())
                                     / (255 * 100);
                    stroke_color = SkColorSetA(stroke_color, fill_alpha);
                    stroke_paint.setColor(stroke_color);
                    stroke_paint.setStrokeWidth(t_value.m_stroke_width);
                    if (context.blend_mode) {
                        stroke_paint.setBlendMode(context.blend_mode.value());
                    }
                    context.canvas.drawTextBlob(blob.get(), x, y, stroke_paint);
                }
                context.restore();
                font_impl = typeface.release();
            }

        } else {
            ALIVE_CORE_ERROR("Could not load font \"{}\"", doc_font.family);
        }
    }
}

void TextLayerNode::update_layer(UpdateContext &context)
{
    LayerNode::update_base_layer_node(context);
    m_text->update(context);
}

const core::Text &TextLayerNode::text() const
{
    return m_text->value();
}

} // namespace alive::model
