#include "text_layer_node.h"
#include <core/asset_management/font_manager.h>
#include <core/color_type.h>
#include <core/composition/paint_context.h>
#include <core/model/layers/text_layer.h>
#include <core/model/property/dynamic_property_animator.h>
#include <core/skia_conversion/skia_to_inae.h>
#include <include/core/SkCanvas.h>
#include <include/core/SkFont.h>
#include <include/core/SkFontMetrics.h>
#include <include/core/SkFontMgr.h>
#include <include/core/SkRefCnt.h>
#include <include/core/SkTextBlob.h>
#include <include/core/SkTypeface.h>

namespace inae::model {

TextLayerNode::TextLayerNode(TextLayer *text_layer)
    : LayerNode(text_layer)
    , m_text_layer(text_layer)
    , m_text(create_animator(text_layer->text(), this))
{}

void TextLayerNode::draw_layer(PaintContext &context) const
{
    const core::TextDocumentProperty &document = m_text->value();
    if (document.m_fill || document.m_stroke) {
        const auto &doc_font = document.m_font;
        const auto &font_mgr = core::FontManager::instance();
        sk_sp<SkTypeface> typeface;
        SkTypeface *font_impl = font_mgr.font_impl(doc_font.family, doc_font.style);
        typeface.reset(font_impl);
        if (typeface) {
            context.save();
            SkRect rect;
            SkFont font(typeface, doc_font.size);
            auto text_width = font.measureText(document.m_text.c_str(),
                                               document.m_text.size(),
                                               SkTextEncoding::kUTF8,
                                               &rect);
            context.canvas.translate(-text_width / 2, 0);

            font.setEdging(SkFont::Edging::kAntiAlias);
            sk_sp<SkTextBlob> blob = SkTextBlob::MakeFromString(document.m_text.c_str(), font);
            if (document.m_fill) {
                SkPaint fill_paint;
                fill_paint.setAntiAlias(true);
                fill_paint.setColor(skcolor_from_vec3d(document.m_fill_color));
                if (context.blend_mode) {
                    fill_paint.setBlendMode(context.blend_mode.value());
                }
                context.canvas.drawTextBlob(blob.get(), 0, 0, fill_paint);
            }

            if (document.m_stroke) {
                SkPaint stroke_paint;
                stroke_paint.setStyle(SkPaint::kStroke_Style);
                stroke_paint.setAntiAlias(true);
                stroke_paint.setStrokeCap(document.m_cap_style);
                stroke_paint.setStrokeJoin(document.m_join_style);
                stroke_paint.setColor(skcolor_from_vec3d(document.m_stroke_color));
                stroke_paint.setStrokeWidth(document.m_stroke_width);
                if (context.blend_mode) {
                    stroke_paint.setBlendMode(context.blend_mode.value());
                }
                context.canvas.drawTextBlob(blob.get(), 0, 0, stroke_paint);
            }
            context.restore();
            font_impl = typeface.release();
        } else {
            INAE_CORE_ERROR("Could not load font \"{}\"", doc_font.family);
        }
    }
}

void TextLayerNode::update_layer(UpdateContext &context)
{
    LayerNode::update_base_layer_node(context);
    m_text->update(context);
}

const core::TextDocumentProperty &TextLayerNode::text() const
{
    return m_text->value();
}

} // namespace inae::model
