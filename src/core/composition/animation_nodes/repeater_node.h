#ifndef REPEATERNODE_H
#define REPEATERNODE_H

#include "shape_transformation_node.h"
#include <core/model/property/dynamic_property.h>
#include <core/model/property_system/property.h>

namespace inae::model {
class Repeater;

class RepeaterNode : public ShapeAnimationNode, public PaintingElement
{
public:
    RepeaterNode(const Repeater *repeater);
    bool update(UpdateContext &context) override;

    void draw(PaintContext &context) const override;

    void set_content(const std::vector<ShapeAnimationNode *> &items_before,
                     const std::vector<ShapeAnimationNode *> &items_after) override;

    bool is_repeater() const override { return true; }

private:
    inae::model::PropertyAnimatorUniquePtr<Vec1D> m_copies;
    inae::model::PropertyAnimatorUniquePtr<Vec1D> m_offset;
    ShapeTransformationNode m_transformation;

    std::vector<PaintingElement *> m_paint_nodes;
};
} // namespace inae::model

#endif // REPEATERNODE_H
