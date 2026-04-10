#include "easing.h"
#include <core/model/keyframes/keyframe.h>

namespace alive::model {
EasingBetweenTwoFramesData::EasingBetweenTwoFramesData(KeyFrame *left, KeyFrame *right)
    : m_left_keyframe(left)
    , m_right_keyframe(right)

{
    auto from_tangent = left->out_tangent();
    m_ct1.x() = from_tangent.x();
    m_ct1.y() = from_tangent.y();

    auto to_tangent = right->in_tangent();
    m_ct2.x() = to_tangent.x();
    m_ct2.y() = to_tangent.y();
}

} // namespace alive::model
