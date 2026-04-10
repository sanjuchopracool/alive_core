#ifndef ANIMATION_NODE_H
#define ANIMATION_NODE_H

#include <core/alive_types/common_types.h>
#include <core/model/property/update_listener.h>
#include <vector>

class QPainter;
namespace alive::script {
class ExpressionEngine;
}

namespace alive::model {
class ShapeItem;
struct UpdateContext
{
    UpdateContext(FrameTimeType t, bool force, script::ExpressionEngine &engine)
        : time(t)
        , force_update(force)
        , expr_engine(engine)
    {}
    const FrameTimeType time;
    const bool force_update;
    script::ExpressionEngine &expr_engine;
};

class AnimationNode : virtual public CascadeUpdateListener
{
public:
    //    ShapeItemNode(ShapeItem *item)
    //        : m_shape_item(item)
    //    {}
    virtual ~AnimationNode() {}
    virtual bool update(UpdateContext &context) = 0;
    inline bool is_static() const { return m_is_static; }

    //

    //    ShapeItem *shape_item() const { return m_shape_item; }

protected:
    bool m_is_static = false;
    //    ShapeItem *m_shape_item = nullptr;
};

class ShapeAnimationNode : public AnimationNode
{
public:
    ShapeAnimationNode(const ShapeItem *item)
        : m_shape_item(item)
    {}

    virtual void set_content(const std::vector<ShapeAnimationNode *> &items_before,
                             const std::vector<ShapeAnimationNode *> &items_after)
    {}
    const ShapeItem *shape_item() const { return m_shape_item; }

protected:
    const ShapeItem *m_shape_item = nullptr;
};

} // namespace alive::model

#endif // ANIMATION_NODE_H
