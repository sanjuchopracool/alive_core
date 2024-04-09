#ifndef TRIMNODE_H
#define TRIMNODE_H

#include "animation_node.h"
#include <core/model/property/dynamic_property.h>
#include <core/model/shape_items/trim.h>

namespace inae::model {
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
    inae::model::PropertyAnimatorUniquePtr<Vec1D> m_start;
    inae::model::PropertyAnimatorUniquePtr<Vec1D> m_end;
    inae::model::PropertyAnimatorUniquePtr<Vec1D> m_offset;
};
} // namespace inae::model
#endif // TRIMNODE_H
