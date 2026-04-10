#ifndef TRIMNODE_H
#define TRIMNODE_H

#include "animation_node.h"
#include <core/model/property/dynamic_property.h>
#include <core/model/shape_items/trim.h>

namespace alive::model {
class TrimNode : public ShapeAnimationNode
{
public:
    TrimNode(const Trim *trim);
    bool update(UpdateContext &context) override;

    Vec1D start() const;
    Vec1D end() const;
    Vec1D offset() const;

    auto trim_type() const { return m_trim->trim_type(); }
    bool hidden() const { return m_trim->hidden(); }

public:
    const Trim *m_trim;
    alive::model::PropertyAnimatorUniquePtr<Vec1D> m_start;
    alive::model::PropertyAnimatorUniquePtr<Vec1D> m_end;
    alive::model::PropertyAnimatorUniquePtr<Vec1D> m_offset;
};
} // namespace alive::model
#endif // TRIMNODE_H
