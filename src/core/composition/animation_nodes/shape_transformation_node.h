#ifndef SHAPETRANSFORMNODE_H
#define SHAPETRANSFORMNODE_H

#include "animation_node.h"
#include <core/composition/interfaces/painting_element.h>
#include <core/model/property/dynamic_property.h>
#include <include/core/SkMatrix.h>

namespace inae::model {
class ShapeTransformation;
class ShapeTransformationNode : public ShapeAnimationNode, public PaintingElement
{
public:
    ShapeTransformationNode(const ShapeTransformation *transformation);

    bool update(UpdateContext &context) override;
    void draw(PaintContext &context) const override;

    const auto &anchor() const { return m_anchor->value(); }
    const auto &position() const { return m_position->value(); }
    const auto &rotation() const { return m_rotation->value(); }
    const auto &scale() const { return m_scale->value(); }
    int opacity() const { return m_opacity->value(); }
    SkMatrix transform() const;

private:
    void update_transform(UpdateContext &context);

private:
    inae::model::PropertyAnimatorUniquePtr<Vec2D> m_anchor;
    inae::model::PropertyAnimatorUniquePtr<Vec2D> m_position;
    inae::model::PropertyAnimatorUniquePtr<Vec2D> m_scale;
    inae::model::PropertyAnimatorUniquePtr<Vec1D> m_rotation;
    inae::model::PropertyAnimatorUniquePtr<Vec1D> m_opacity;
    inae::model::PropertyAnimatorUniquePtr<Vec1D> m_skew;
    inae::model::PropertyAnimatorUniquePtr<Vec1D> m_skew_axis;
};

} // namespace inae::model

#endif // SHAPETRANSFORMNODE_H
