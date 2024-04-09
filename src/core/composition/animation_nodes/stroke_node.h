#ifndef STROKE_NODE_H
#define STROKE_NODE_H

#include "animation_node.h"
#include <core/composition/interfaces/painting_element.h>
#include <include/core/SkColor.h>
#include <core/model/property/dynamic_property.h>

namespace inae::model {
class Stroke;
class PathNode;

class StrokeNode : public ShapeAnimationNode, public PaintingElement
{
public:
    StrokeNode(const Stroke *stroke);
    bool update(UpdateContext &context) override;

    //    void render(QPainter *painter) override;
    void set_content(const std::vector<ShapeAnimationNode *> &,
                     const std::vector<ShapeAnimationNode *> &items_after) override;

    void draw(PaintContext &context) const override;

private:
    const Stroke *m_stroke;
    inae::model::PropertyAnimatorUniquePtr<Vec1D> m_width;
    inae::model::PropertyAnimatorUniquePtr<Vec1D> m_opacity;
    inae::model::PropertyAnimatorUniquePtr<Vec3D> m_color;
    //    std::vector<DashElement> m_dashPattern;
    std::vector<const PathNode *> m_paths;
};
} // namespace inae::model

#endif // STROKE_NODE_H
