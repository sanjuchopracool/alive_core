#ifndef TRANSFORMNODE_H
#define TRANSFORMNODE_H

#include "animation_node.h"
#include <include/core/SkMatrix.h>
#include <core/model/property/dynamic_property.h>

/*
 * There is only one transformation possible for a group in after effects
 */

namespace inae::model {
class Transform;

class LayerTransformationNode : public AnimationNode
{
public:
    LayerTransformationNode(const Transform &transformation);

    bool update(UpdateContext &context) override;

    SkMatrix transform() const;
    int opacity() const { return m_opacity->value(); }

    const auto &anchor() const { return m_anchor->value(); }
    const auto &position() const { return m_position->value(); }
    const auto &rotation() const { return m_rotation->value(); }
    const auto &scale() const { return m_scale->value(); }

private:
    void update_transform(UpdateContext &context);

private:
    inae::model::PropertyAnimatorUniquePtr<Vec3D> m_anchor;
    inae::model::PropertyAnimatorUniquePtr<Vec3D> m_position;
    inae::model::PropertyAnimatorUniquePtr<Vec3D> m_scale;
    inae::model::PropertyAnimatorUniquePtr<float> m_rotation;
    inae::model::PropertyAnimatorUniquePtr<float> m_opacity;
};
} // namespace inae::model

#endif // TRANSFORMNODE_H
