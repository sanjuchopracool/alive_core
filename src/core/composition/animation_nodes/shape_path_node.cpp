#include "shape_path_node.h"
#include "compound_trim_path.h"
#include "trim_node.h"

namespace inae::model {

ShapePathNode::ShapePathNode(const ShapeItem *item)
    : ShapeAnimationNode(item)
{}

ShapePathNode::~ShapePathNode() {}

void ShapePathNode::set_content(const std::vector<ShapeAnimationNode *> &items_before,
                                const std::vector<ShapeAnimationNode *> &items_after)
{
    if (m_compound_trim) {
        m_compound_trim->clear();
    }
    for (ShapeAnimationNode *item : items_before) {
        TrimNode *trim = dynamic_cast<TrimNode *>(item);
        if (trim && trim->trim_type() == Trim::TrimType::Simultaneously) {
            if (!m_compound_trim) {
                m_compound_trim = std::make_unique<CompoundTrimPath>();
            }
            observe(trim);
            m_compound_trim->add_trim_node(trim);
        }
    }
}

void ShapePathNode::apply_trim()
{
    if (m_compound_trim) {
        m_compound_trim->apply_trim(m_path);
    }
}

} // namespace inae::model
