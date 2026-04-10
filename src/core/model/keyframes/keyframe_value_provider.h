#ifndef KEYFRAMEVALUEPROVIDER_H
#define KEYFRAMEVALUEPROVIDER_H

#include "keyframe.h"
#include <Magnum/Math/Functions.h>
#include <core/script/expression_engine.h>
#include <core/script/expression_value_provider.h>
namespace alive::model {

template<typename T>
class KeyFrameValueProvider
{
public:
    using valueType = T;
    using KeyFrameType = KeyFrame;

    KeyFrameValueProvider(const KeyFrames &frames)
        : m_keyframes(frames)
    {
        if (m_keyframes.size()) {
            m_value = static_cast<const KeyFrameTyped<T> *>(m_keyframes.front())->value();
        }
    }

    const T &value() const { return m_value; }

    bool has_update(FrameTimeType time) const
    {
        if (m_last_updated_frame == -1)
            return true;

        //        if (m_keyframes.size() == 1)
        //            return false;

        //        if (is_valid_index(m_trailing_index) and (not is_valid_index(m_leading_index))) {
        //            if (time < m_keyframes[m_trailing_index]->time())
        //                return false;
        //        }

        //        if (is_valid_index(m_leading_index) and (not is_valid_index(m_trailing_index))) {
        //            if (m_keyframes[m_leading_index]->time() < time)
        //                return false;
        //        }

        //        if (is_valid_index(m_leading_index) and is_valid_index(m_trailing_index)
        //            and m_keyframes[m_leading_index]->is_hold()) {
        //            if ((m_keyframes[m_leading_index]->time() < time)
        //                and (time < m_keyframes[m_trailing_index]->time()))
        //                return true;
        //        }

        return true;
    }

    void update(FrameTimeType time, script::ExpressionEngine &expr_engine)
    {
        update_indices(time);
        if (is_valid_index(m_leading_index) and is_valid_index(m_trailing_index)) {
            // interpolate
            const KeyFrameTyped<T> *leading_keyframe = static_cast<const KeyFrameTyped<T> *>(
                m_keyframes[m_leading_index]);
            const KeyFrameTyped<T> *trailing_keyframe = static_cast<const KeyFrameTyped<T> *>(
                m_keyframes[m_trailing_index]);
            float progress = trailing_keyframe->interpolated_progress(leading_keyframe, time);
            const auto from_value = key_frame_value(trailing_keyframe, expr_engine);
            const auto to_value = key_frame_value(leading_keyframe, expr_engine);
            m_value = Magnum::Math::lerp(from_value, to_value, progress);
        } else if (is_valid_index(m_leading_index)) {
            const KeyFrameTyped<T> *leading_keyframe = static_cast<const KeyFrameTyped<T> *>(
                m_keyframes[m_leading_index]);
            m_value = key_frame_value(leading_keyframe, expr_engine);
        } else if (is_valid_index(m_trailing_index)) {
            const KeyFrameTyped<T> *trailing_keyframe = static_cast<const KeyFrameTyped<T> *>(
                m_keyframes[m_trailing_index]);
            m_value = key_frame_value(trailing_keyframe, expr_engine);
        } else {
            const KeyFrameTyped<T> *keyframe = static_cast<const KeyFrameTyped<T> *>(m_keyframes[0]);
            m_value = key_frame_value(keyframe, expr_engine);
        }
    }

    bool is_static() const { return (m_keyframes.size() == 1); }

    bool is_valid_expression(script::ExpressionEngineInterface *engine,
                             const std::string &expression)
    {
        return m_expression_value_provider.is_valid(static_cast<script::ExpressionEngine *>(engine),
                                                    expression);
    }

    const T &key_frame_value(const KeyFrameTyped<T> *keyframe, script::ExpressionEngine &expr_engine)
    {
        if (keyframe->expression_enabled()) {
            const auto &exp = keyframe->expression();
            if (!exp.empty() && m_expression_value_provider.evaluate(expr_engine, exp)) {
                return m_expression_value_provider.value();
            }
        }

        return keyframe->value();
    }

private:
    void update_indices(FrameTimeType time)
    {
        m_last_updated_frame = time;

        if (m_keyframes.size() == 1) {
            m_leading_index = 0;
            m_trailing_index = -1;
            return;
        }

        if (-1 == m_leading_index && -1 == m_trailing_index) {
            if (time < m_keyframes[0]->time())
                m_trailing_index = 0;
            else {
                m_leading_index = 0;
                m_trailing_index = 1;
            }
        }

        m_leading_index = 0;
        m_trailing_index = -1;

        const auto last_index = (m_keyframes.size() - 1);
        int i = 0;
        for (; i < last_index; ++i) {
            const auto &keyframe = m_keyframes[i];
            const auto &next_keyframe = m_keyframes[i + 1];
            if (time <= keyframe->time()) {
                m_leading_index = i;
                m_trailing_index = i - 1;
                break;
            } else if (next_keyframe->time() == time) {
                m_leading_index = i + 1;
                m_trailing_index = i;
                break;
            } else if ((time > keyframe->time()) && (time < next_keyframe->time())) {
                m_leading_index = i + 1;
                m_trailing_index = i;
                break;
            }
        }

        if (i >= last_index) {
            m_trailing_index = last_index;
            m_leading_index = -1;
        }
    }

    int valid_index(int index) const
    {
        return (m_keyframes.size() > index) ? (index >= 0) ? index : -1 : -1;
    }

    bool is_valid_index(int index) const { return (index >= 0) and index < m_keyframes.size(); }

private:
    T m_value = {};

    const KeyFrames &m_keyframes;

    FrameTimeType m_last_updated_frame = -1;
    int m_leading_index = -1;
    int m_trailing_index = -1;
    script::ExpressionValueProvider<T> m_expression_value_provider;
};
} // namespace alive::model

#endif // KEYFRAMEVALUEPROVIDER_H
