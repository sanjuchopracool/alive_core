#include "transformation_node.h"

#include <core/model/property/dynamic_property_animator.h>
#include <core/model/transform.h>

namespace alive::model {

LayerTransformationNode::LayerTransformationNode(const Transform &transformation)
    : m_anchor(create_animator(transformation.anchor(), this))
    , m_position(create_animator(transformation.position(), this))
    , m_scale(create_animator(transformation.scale(), this))
    , m_rotation(create_animator(transformation.rotation(), this))
    , m_opacity(create_animator(transformation.opacity(), this))
    , m_skew(create_animator(transformation.skew(), this))
    , m_skew_axis(create_animator(transformation.skew_axis(), this))
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
    m_skew->update(context);
    m_skew_axis->update(context);
}

SkMatrix LayerTransformationNode::transform() const
{
    auto anchor = m_anchor->value();
    auto pos = m_position->value();
    auto rotation = m_rotation->value();
    auto scale = m_scale->value() / 100.0f;
    auto skew_angle = m_skew->value();
    auto skew_axis = m_skew_axis->value();

    if (skew_angle != 0.0f) {
        skew_angle = fmod(skew_angle, 180.0f);
        if (fabsf(skew_angle - 90.0f) < 0.01f || fabsf(skew_angle + 90.0f) < 0.01f) {
            return SkMatrix::Scale(0, 0);
        }
    }

    SkMatrix tr;
    tr.preTranslate(pos.x(), pos.y());
    tr.preRotate(rotation);
    if (skew_angle != 0.0f) {
        using namespace Magnum::Math;
        tr.preRotate(-skew_axis);
        tr.preSkew(tan(-Constants<float>::pi() * skew_angle / 180), 0.0f);
        tr.preRotate(skew_axis);
    }
    tr.preScale(scale.x(), scale.y());
    tr.preTranslate(-anchor.x(), -anchor.y());

    return tr;
}

} // namespace alive::model
