#ifndef EASINGDATA_H
#define EASINGDATA_H

#include <core/alive_types/common_types.h>

namespace alive::model {
class CompositionDocument;
class Property;
class KeyFrame;

struct EasingBetweenTwoFramesData
{
public:
    EasingBetweenTwoFramesData() {}
    EasingBetweenTwoFramesData(KeyFrame *left, KeyFrame *right);

    KeyFrame *m_left_keyframe = nullptr;
    KeyFrame *m_right_keyframe = nullptr;
    Vec2D m_ct1;
    Vec2D m_ct2;
};
} // namespace alive::model
#endif // EASINGDATA_H
