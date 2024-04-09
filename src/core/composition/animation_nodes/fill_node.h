#ifndef FILL_NODE_H
#define FILL_NODE_H

#include "animation_node.h"
#include <core/composition/interfaces/painting_element.h>
#include <include/core/SkColor.h>
#include <core/model/property/dynamic_property.h>

namespace inae::model {
class Fill;
class PathNode;

class FillNode : public ShapeAnimationNode, public PaintingElement
{
public:
    FillNode(const Fill *fill);
    bool update(UpdateContext &context) override;

    void set_content(const std::vector<ShapeAnimationNode *> &,
                     const std::vector<ShapeAnimationNode *> &items_after) override;

    void draw(PaintContext &context) const override;

private:
    const Fill *m_fill;
    inae::model::PropertyAnimatorUniquePtr<Vec3D> m_color;
    inae::model::PropertyAnimatorUniquePtr<Vec1D> m_opacity;
    std::vector<const PathNode *> m_paths;
};
} // namespace inae::model

#endif // FILL_NODE_H
