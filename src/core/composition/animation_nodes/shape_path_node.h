#ifndef SHAPE_PATH_NODE_H
#define SHAPE_PATH_NODE_H

#include "animation_node.h"
#include "path_node.h"
#include <memory>

namespace inae::model {
class CompoundTrimPath;

class ShapePathNode : public PathNode, public ShapeAnimationNode
{
public:
    ShapePathNode(const ShapeItem *item);
    ~ShapePathNode() override;

protected:
    void set_content(const std::vector<ShapeAnimationNode *> &items_before,
                     const std::vector<ShapeAnimationNode *> &items_after) override;
    void apply_trim();
    std::unique_ptr<CompoundTrimPath> m_compound_trim;
};

} // namespace inae::model

#endif // SHAPE_PATH_NODE_H
