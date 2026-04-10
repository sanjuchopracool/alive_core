#include "shape_node.h"
#include <core/model/shape_items/shape.h>
#include <core/model/property/dynamic_property_animator.h>

namespace alive::model {

ShapeNode::ShapeNode(const Shape *shape)
    : ShapePathNode(shape)
    , m_shape(shape)
    , m_pathProp(create_animator(shape->path(), this))
{
}

bool ShapeNode::update(UpdateContext &context)
{
    bool result = false;
    m_pathProp->update(context);
    // TODO listen to trim paths also
    if (context.force_update || m_dirty || m_shape->is_dirty()) {
        m_path = m_pathProp->value().path();
        apply_trim();
        m_dirty = false;
        result = true;
    }
    return result;
}
} // namespace alive::model
