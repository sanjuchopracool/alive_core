#include "layer_node.h"
#include "animation_nodes/transformation_node.h"
#include "shape_layer_node.h"
#include "solid_layer_node.h"
#include "text_layer_node.h"
#include <algorithm>
#include <core/composition/animation_nodes/animation_node.h>
#include <core/composition/composition_utility.h>
#include <core/composition/paint_context.h>
#include <core/model/layers/layer.h>
#include <core/model/transform.h>
#include <include/core/SkBitmap.h>
#include <include/core/SkCanvas.h>
#include <include/core/SkData.h>
#include <include/core/SkImage.h>
#include <include/core/SkStream.h>
#include <include/core/SkSurface.h>
#include <include/encode/SkPngEncoder.h>

namespace alive::model {

void concat_parent_transformation(PaintContext &context, const LayerNode *parent)
{
    if (parent) {
        concat_parent_transformation(context, parent->parent_layer());
        SkMatrix tr;
        parent->get_transform(tr);
        context.canvas.concat(tr);
        context.parent_alpha = context.parent_alpha * parent->opacity() / 100;
    }
}

LayerNode::LayerNode(Layer *layer)
    : m_layer(layer)
    , m_transform(std::make_unique<LayerTransformationNode>(layer->transform()))
{
    observe(layer);
    observe(m_transform.get());
}

LayerNode::~LayerNode() {}

FrameTimeType LayerNode::local_frame(FrameTimeType t) const
{
    return t;
}

void LayerNode::draw(PaintContext &context) const
{
    if (!m_debug_visible)
        return;

    if (!context.is_matte_context && (m_layer->is_hidden() || !m_visible))
        return;

    int matte_depth = 0;
    const LayerNode *layer = this;
    while (layer->has_matte()) {
        ++matte_depth;
        layer = layer->matte_layer();
    }

    if (!context.is_matte_context && matte_depth > 1) {
        draw_nested_layer(context);
        return;
    }
    PaintContextGuard g(context);
    if (has_matte() && !context.is_matte_context) {
        auto matte_context = context;
        SkPaint p;
        p.setAntiAlias(true);
        auto blend_mode = to_sk_blend_mode(m_layer->blend_mode());
        if (blend_mode) {
            p.setBlendMode(blend_mode.value());
        }
        matte_context.canvas.saveLayer(nullptr, &p);
        {
            matte_context.is_matte_context = true;
            matte_context.canvas.clear(SK_ColorTRANSPARENT);
            // draw layer
            this->draw(matte_context);

            SkPaint p;
            p.setAntiAlias(true);
            if (m_layer->matte_type() == MatteType::e_AlphaInvert) {
                p.setBlendMode(SkBlendMode::kDstIn);
            } else {
                p.setBlendMode(SkBlendMode::kDstOut);
            }
            matte_context.canvas.saveLayer(nullptr, &p);
            matte_context.canvas.clear(SK_ColorWHITE);
            matte_context.blend_mode = SkBlendMode::kClear;
            m_matte_layer->draw(matte_context);
            matte_context.canvas.restore();
        }
        matte_context.canvas.restore();
    } else {
        auto new_context = context;
        concat_parent_transformation(new_context, m_parent_layer);
        new_context.parent_alpha = new_context.parent_alpha * m_transform->opacity() / 100;
        new_context.canvas.concat(m_transform->transform());
        if (!context.is_matte_context) {
            auto blend_mode = to_sk_blend_mode(m_layer->blend_mode());
            if (blend_mode) {
                new_context.blend_mode = blend_mode;
            }
        }
        draw_layer(new_context);
    }
}

void LayerNode::draw_nested_matte_layers(PaintContext &context) const
{
    SkPaint p2;
    p2.setAntiAlias(true);
    PaintContextGuard g(context);
    // PaintContextSaveLayerGuard g(context, p2);
    {
        const auto *layer = this;
        std::vector<const LayerNode *> layers;
        layers.emplace_back(layer);
        while (layer->has_matte()) {
            const auto *matte_layer = layer->matte_layer();
            layer = matte_layer;
            layers.emplace_back(layer);
        }

        // make it matte -> layer by reversing vector
        std::reverse(layers.begin(), layers.end());
        {
            SkPaint paint;
            paint.setAntiAlias(true);

            auto info = context.canvas.imageInfo();
            std::vector<SkBitmap> bitmaps; //matte to layer
            size_t max_index = layers.size() -1;
            for(size_t i = 0; i <= max_index ; ++i) {
                const auto* layer = layers.at(i);
                {
                    auto new_context = context;
                    new_context.canvas.clear(SK_ColorTRANSPARENT);
                    new_context.blend_mode = SkBlendMode::kSrcOver;
                    layer->draw_matte_layer(new_context);
                }
                bitmaps.emplace_back();
                auto &bitmap = bitmaps.back();
                bitmap.allocPixels(SkImageInfo::MakeN32Premul(info.width(), info.height()));
                context.canvas.readPixels(bitmap, 0, 0);
                bitmap.setImmutable();

                /*{
                    sk_sp<SkData> png = SkPngEncoder::Encode(nullptr,
                                                             bitmap.asImage().get(),
                                                             SkPngEncoder::Options());
                    if (!png) {
                        ALIVE_CORE_CRITICAL("could not create png from snapshot {}", i);
                        return;
                    }

                    std::string path (std::format("/Users/sanju/image_{}.png", i));
                    SkFILEWStream out(path.c_str());
                    (void) out.write(png->data(), png->size());
                }*/
            }
            // reset scale because we are just drawing scaled image over image
            context.canvas.resetMatrix();
            context.canvas.clear(SK_ColorTRANSPARENT);
            auto& bitmap = bitmaps[0];
            auto image = bitmap.asImage();
            paint.setBlendMode(SkBlendMode::kSrc);
            context.canvas.drawImage(image.get(), 0, 0, SkSamplingOptions(), &paint);
            for(size_t i = 1; i <= max_index ; ++i) {
                auto& bitmap = bitmaps[i];
                bitmap.setImmutable();
                auto img = bitmap.asImage();
                paint.setBlendMode(SkBlendMode::kSrcIn); // Set the desired blend mode
                context.canvas.drawImage(img.get(), 0, 0, SkSamplingOptions(), &paint);
            }

            bitmap.allocPixels(SkImageInfo::MakeN32Premul(info.width(), info.height()));
            context.canvas.readPixels(bitmap, 0, 0);
            bitmap.setImmutable();

            /*{
                sk_sp<SkData> png = SkPngEncoder::Encode(nullptr,
                                                         bitmap.asImage().get(),
                                                         SkPngEncoder::Options());
                if (!png) {
                    ALIVE_CORE_CRITICAL("could not create png from snapshot {}", 0);
                    return;
                }

                std::string path (std::format("/Users/sanju/final_image.png"));
                SkFILEWStream out(path.c_str());
                (void) out.write(png->data(), png->size());
            }*/
        }
    }
}

void LayerNode::draw_nested_layer(PaintContext &context) const
{
    auto info = context.canvas.imageInfo();
    SkBitmap bitmap;
    bitmap.allocN32Pixels(info.width(), info.height());
    SkCanvas canvas(bitmap);

    PaintContext new_context(canvas, context.parent_alpha);
    new_context.is_matte_context = context.is_matte_context;
    new_context.blend_mode = context.blend_mode;

    auto matrix = context.canvas.getTotalMatrix();
    canvas.setMatrix(matrix);

    SkPaint p;
    p.setAntiAlias(true);
    auto blend_mode = to_sk_blend_mode(m_layer->blend_mode());
    if (blend_mode) {
        p.setBlendMode(blend_mode.value());
    }
    draw_nested_matte_layers(new_context);
    bitmap.setImmutable();
    {
        PaintContextGuard g(context);
        context.canvas.resetMatrix();
        context.canvas.drawImage(bitmap.asImage(), 0, 0, SkSamplingOptions(), &p);
    }
}

void LayerNode::draw_matte_layer(PaintContext &context) const
{
    auto new_context = context;
    PaintContextGuard g(new_context);
    concat_parent_transformation(new_context, m_parent_layer);
    new_context.parent_alpha = new_context.parent_alpha * m_transform->opacity() / 100;
    new_context.canvas.concat(m_transform->transform());
    if (!context.is_matte_context) {
        auto blend_mode = to_sk_blend_mode(m_layer->blend_mode());
        if (blend_mode) {
            new_context.blend_mode = blend_mode;
        }
    }
    draw_layer(new_context);
}

void LayerNode::get_transform(SkMatrix &o_transform) const
{
    o_transform = m_transform->transform();
}

void LayerNode::update_base_layer_node(UpdateContext &context)
{
    // TODO check hidden layer or in range update
    // also start time
    bool visible = m_layer->in_range(context.time);
    if (m_visible != visible) {
        m_visible = visible;
        CascadeUpdateListener::notify_observers();
    }
    // if (!m_visible)
    //     return;

    m_dirty = false;
    UpdateContext new_context(context.time, context.force_update, context.expr_engine);
    m_transform->update(new_context);
}

int LayerNode::opacity() const
{
    return m_transform->opacity();
}

const std::string &LayerNode::name() const
{
    return m_layer->name();
}

void LayerNode::set_debug_visible(bool visible)
{
    m_debug_visible = visible;
    CascadeUpdateListener::notify_observers();
}

LayerType LayerNode::layer_type() const
{
    return m_layer->layer_type();
}

int LayerNode::layer_index() const
{
    return m_layer->layer_index();
}

int LayerNode::matte_layer_index() const
{
    return m_layer->matte_layer_index();
}

int alive::model::LayerNode::parent_layer_index() const
{
    return m_layer->parent_layer_index();
}

bool LayerNode::has_matte() const
{
    return (m_layer->matte_type() != MatteType::e_None && m_matte_layer);
}

SolidLayerNode *LayerNode::to_solid_layer()
{
    if (layer_type() == LayerType::e_Solid) {
        return static_cast<SolidLayerNode *>(this);
    }
    return nullptr;
}

TextLayerNode *LayerNode::to_text_layer()
{
    if (layer_type() == LayerType::e_Text) {
        return static_cast<TextLayerNode *>(this);
    }
    return nullptr;
}

ShapeLayerNode *LayerNode::to_shape_layer()
{
    if (layer_type() == LayerType::e_Shape) {
        return static_cast<ShapeLayerNode *>(this);
    }
    return nullptr;
}

const SolidLayerNode *LayerNode::to_solid_layer() const
{
    return const_cast<LayerNode *>(this)->to_solid_layer();
}

const TextLayerNode *LayerNode::to_text_layer() const
{
    return const_cast<LayerNode *>(this)->to_text_layer();
}

const ShapeLayerNode *LayerNode::to_shape_layer() const
{
    return const_cast<LayerNode *>(this)->to_shape_layer();
}

} // namespace alive::model
