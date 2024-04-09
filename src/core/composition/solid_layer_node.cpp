#include "solid_layer_node.h"
#include <core/composition/animation_nodes/transformation_node.h>
#include <core/composition/paint_context.h>
#include <include/core/SkCanvas.h>
#include <core/model/layers/solid_layer.h>

namespace inae::model {
SolidLayerNode::SolidLayerNode(SolidLayer *solid_layer)
    : LayerNode(solid_layer)
    , m_layer(solid_layer)
{}

void SolidLayerNode::draw_layer(PaintContext &context) const
{
    SkColor color = m_layer->color();

    if (SkColorGetA(color) == 0 || opacity() == 0)
        return;

    int alpha = (context.parent_alpha * SkColorGetA(color) * opacity()) / (255 * 100);
    color = SkColorSetA(color, alpha);

    SkPaint paint;
    paint.setAntiAlias(true);
    paint.setColor(color);
    if (context.blend_mode) {
        paint.setBlendMode(context.blend_mode.value());
    }
    context.canvas.drawRect(SkRect::MakeXYWH(0, 0, m_layer->width(), m_layer->height()), paint);
}

int SolidLayerNode::width() const
{
    return m_layer->width();
}

int SolidLayerNode::height() const
{
    return m_layer->height();
}

const SkColor &SolidLayerNode::color() const
{
    return m_layer->color();
}

void SolidLayerNode::update_layer(UpdateContext &context)
{
    update_base_layer_node(context);
}
} // namespace inae::model
