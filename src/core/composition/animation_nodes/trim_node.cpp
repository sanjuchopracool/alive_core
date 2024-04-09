#include "trim_node.h"


#include <core/model/shape_items/trim.h>
#include <core/model/property/dynamic_property_animator.h>
#include <core/profiler.h>

namespace inae::model {
TrimNode::TrimNode(const Trim *trim)
    : ShapeAnimationNode(trim)
    , m_trim(trim)
    , m_start(create_animator(trim->start(), this))
    , m_end(create_animator(trim->end(), this))
    , m_offset(create_animator(trim->offset(), this))
{
}

bool TrimNode::update(UpdateContext &context)
{
    m_dirty = false;
    m_start->update(context);
    m_end->update(context);
    m_offset->update(context);
    return m_dirty;
}

Vec1D TrimNode::start() const
{
    return m_start->value();
}

Vec1D TrimNode::end() const
{
    return m_end->value();
}

Vec1D TrimNode::offset() const
{
    return m_offset->value();
}
} // namespace inae::model
