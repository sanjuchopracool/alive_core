#include "repeater_node.h"

#include <cmath>
#include <core/composition/paint_context.h>
#include <include/core/SkCanvas.h>
#include <core/model/property/dynamic_property_animator.h>
#include <core/model/shape_items/repeater.h>
#include <ranges>

namespace inae::model {
RepeaterNode::RepeaterNode(const Repeater *repeater)
    : ShapeAnimationNode(repeater)
    , m_copies(create_animator(repeater->copies(), this))
    , m_offset(create_animator(repeater->offset(), this))
    , m_transformation(repeater->transform())
{
}

bool RepeaterNode::update(UpdateContext &context)
{
    m_dirty = m_shape_item->is_dirty();
    m_copies->update(context);
    m_offset->update(context);
    m_transformation.update(context);
    return m_dirty;
}

void RepeaterNode::draw(PaintContext &context) const
{
    if (m_shape_item->hidden())
        return;

    context.save();
    int offset = std::roundf(m_offset->value());
    int copies = std::roundf(m_copies->value());
    for (int i = 0; i < copies + offset; ++i) {
        if (i >= offset) {
            for (auto *item : m_paint_nodes) {
                item->draw(context);

                if (item->is_repeater()) {
                    break;
                }
            }
        }
        m_transformation.draw(context);
    }
    context.restore();
}

void RepeaterNode::set_content(const std::vector<ShapeAnimationNode *> &items_before,
                               const std::vector<ShapeAnimationNode *> &items_after)
{
    m_paint_nodes.clear();
    std::ranges::reverse_view reverse(items_after);
    for (auto *node : reverse) {
        auto *paint_element = dynamic_cast<PaintingElement *>(node);
        if (paint_element) {
            m_paint_nodes.emplace_back(paint_element);
        }
    }
}
} // namespace inae::model
