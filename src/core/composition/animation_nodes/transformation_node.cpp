#include "transformation_node.h"

#include <core/model/property/dynamic_property_animator.h>
#include <core/model/transform.h>

namespace inae::model {

LayerTransformationNode::LayerTransformationNode(const Transform &transformation)
    : m_anchor(create_animator(transformation.anchor(), this))
    , m_position(create_animator(transformation.position(), this))
    , m_scale(create_animator(transformation.scale(), this))
    , m_rotation(create_animator(transformation.rotation(), this))
    , m_opacity(create_animator(transformation.opacity(), this))

{}

bool LayerTransformationNode::update(UpdateContext &context)
{
    m_dirty = false;
    update_transform(context);
    return m_dirty;
}

void LayerTransformationNode::update_transform(UpdateContext &context)
{
    m_anchor->update(context);
    m_position->update(context);
    m_rotation->update(context);
    m_scale->update(context);
    m_opacity->update(context);
}

SkMatrix LayerTransformationNode::transform() const
{
    auto anchor = m_anchor->value();
    auto pos = m_position->value();
    auto rotation = m_rotation->value();
    auto scale = m_scale->value() / 100.0f;

    SkMatrix tr;
    //    canvas->translate(pos.x, pos.y);
    //    canvas->rotate(rotation);
    //    canvas->scale(scale.x, scale.y);
    //    canvas->translate(-anchor.x, -anchor.y);
    tr.preTranslate(pos.x(), pos.y())
        .preRotate(rotation)
        .preScale(scale.x(), scale.y())
        .preTranslate(-anchor.x(), -anchor.y());

    return tr;
}

} // namespace inae::model
