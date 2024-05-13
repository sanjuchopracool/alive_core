#include "layer_node.h"
#include "animation_nodes/transformation_node.h"
#include <core/model/layers/layer.h>
#include "shape_layer_node.h"
#include "solid_layer_node.h"
#include "text_layer_node.h"
#include <core/composition/animation_nodes/animation_node.h>
#include <core/composition/composition_utility.h>
#include <core/composition/paint_context.h>
#include <include/core/SkBitmap.h>
#include <include/core/SkCanvas.h>
#include <include/core/SkData.h>
#include <include/core/SkImage.h>
#include <include/core/SkStream.h>
#include <include/core/SkSurface.h>
#include <include/encode/SkPngEncoder.h>
#include <core/model/transform.h>

namespace inae::model {

void concat_parent_transformation(SkCanvas &canvas, const LayerNode *parent)
{
    if (parent) {
        concat_parent_transformation(canvas, parent->parent_layer());
        canvas.concat(parent->transform());
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

    context.save();
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
        concat_parent_transformation(new_context.canvas, m_parent_layer);
        new_context.parent_alpha = context.parent_alpha * m_transform->opacity() / 100;
        new_context.canvas.concat(m_transform->transform());
        if (!context.is_matte_context) {
            auto blend_mode = to_sk_blend_mode(m_layer->blend_mode());
            if (blend_mode) {
                new_context.blend_mode = blend_mode;
            }
        }
        draw_layer(new_context);
    }
    context.restore();
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

SkMatrix LayerNode::transform() const
{
    return m_transform->transform();
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

int inae::model::LayerNode::parent_layer_index() const
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

} // namespace inae::model
