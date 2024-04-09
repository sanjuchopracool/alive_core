#ifndef DYNAMIC_PROPERTY_H
#define DYNAMIC_PROPERTY_H

#include "property.h"
#include "property_animator.h"
#include "property_value_validator.h"
#include <algorithm>
#include <core/model/keyframes/keyframe.h>

#include <memory>

namespace inae::model {
const char k_no_animator_message[] = "Atleast one property animator should be there!";

template<typename T>
class DynamicProperty : public PropertyTemplate<T>
{
public:
    using creference = const T &;
    using reference = T &;

    DynamicProperty(Object *object,
                    const PropertyDescriptor &descriptor,
                    const T &default_value = {},
                    const PropertyValueValidator<T> *validator = nullptr)
        : PropertyTemplate<T>(object, descriptor)
        , m_validator(validator)
    {
        set_value(default_value);
    }

    ~DynamicProperty()
    {
        for (auto *item : m_keyframes) {
            delete item;
        }
        m_keyframes.clear();
    }

    bool validate(reference value) const
    {
        if (m_validator) {
            return m_validator->validate(value);
        }

        return true;
    }

    creference get_value() const override
    {
        if (m_animation_type == PropertyAnimationType::e_Constant && m_expression.empty()) {
            return m_value;
        } else {
            assertm(m_animators.size(), k_no_animator_message);
            return m_animators[0]->value();
        }
    }

    KeyFrame *set_value(creference value, KeyFrameTime t = 0) override
    {
        T validated_value = value;
        if (m_validator) {
            m_validator->validate(validated_value);
        }
        if (m_animation_type == PropertyAnimationType::e_Constant) {
            if (m_value != validated_value) {
                m_value = validated_value;
                notify();
            }
            return nullptr;
        }
        return set_keyframe_value(validated_value, t);
    }

    /******************************************************************************************************************/
    /*                                     EXPRESSION RELATED CODE                                                      */
    /******************************************************************************************************************/

    KeyFrame *set_keyframe_expression(const std::string &expr, KeyFrameTime t)
    {
        KeyFrame *keyframe = nullptr;
        if (m_animation_type == PropertyAnimationType::e_KeyFramed) {
            for (auto frame : m_keyframes) {
                if (t == frame->time()) {
                    keyframe = frame;
                    break;
                }
            }

            if (!keyframe) {
                keyframe = add_keyframe(t, get_value());
            }

            if (keyframe) {
                keyframe->set_expression(expr);
                notify();
            }
        } else {
            m_expression = expr;
            notify();
        }

        return keyframe;
    }

    virtual KeyFrame *set_expression(const std::string &script, KeyFrameTime time) override
    {
        return set_keyframe_expression(script, time);
    }
    const std::string &get_expression(KeyFrameTime time) const override
    {
        if (m_animation_type == PropertyAnimationType::e_KeyFramed) {
            for (auto frame : m_keyframes) {
                if (time == frame->time()) {
                    return frame->expression();
                }
            }
        }

        return m_expression;
    }

    bool is_valid_expression(script::ExpressionEngineInterface *engine,
                             const std::string &expression) override
    {
        return m_animators[0]->is_valid_expression(engine, expression);
    }

    /******************************************************************************************************************/
    /*                                     VALUE TYPE RELATED CODE                                                      */
    /******************************************************************************************************************/
    PropertyAnimationType value_type() const override { return m_animation_type; }
    void set_value_type(PropertyAnimationType type) override { m_animation_type = type; }

    void add_animator(PropertyAnimatorPtr<T> animator) const { m_animators.emplace_back(animator); }
    void remove_animator(PropertyAnimatorPtr<T> animator) const
    {
        m_animators.erase(std::remove(m_animators.begin(), m_animators.end(), animator),
                          m_animators.end());
    }

    virtual bool is_animating() const override
    {
        return m_animation_type == PropertyAnimationType::e_KeyFramed;
    }
    void notify()
    {
        for (auto &animatior : m_animators)
            animatior->notify();
        Property::notify_observers();
    }

    /******************************************************************************************************************/
    /*                                     KEYFRAME RELATED CODE                                                      */
    /******************************************************************************************************************/

    void set_keyframes(KeyFrames &keyframes) override
    {
        m_animation_type = PropertyAnimationType::e_KeyFramed;
        m_keyframes = std::move(keyframes);
        notify();
    }

    KeyFrame *set_keyframe_value(creference value, KeyFrameTime t)
    {
        for (auto keyframe : m_keyframes) {
            if (t == keyframe->time()) {
                static_cast<KeyFrameTyped<T> *>(keyframe)->set_value(value);
                notify();
                return nullptr;
            }
        }

        return add_keyframe(t, value);
    }

    const KeyFrames &get_keyframes() const override { return m_keyframes; }
    KeyFrames &get_keyframes() override { return m_keyframes; }

    virtual KeyFrames remove_all_keyframes() override
    {
        KeyFrames frames = m_keyframes;
        assertm(m_animators.size(), k_no_animator_message);
        m_value = m_animators[0]->value();
        m_keyframes.clear();
        m_animation_type = PropertyAnimationType::e_Constant;
        notify();
        return frames;
    }

    KeyFrame *add_keyframe(KeyFrameTime t) override { return add_keyframe(t, get_value()); }

    void sort_keyframes_by_time() override
    {
        assert(m_animation_type == PropertyAnimationType::e_KeyFramed);
        sort(m_keyframes.begin(), m_keyframes.end(), [](const KeyFrame *t1, const KeyFrame *t2) {
            return t1->time() < t2->time();
        });
        notify();
    }
    KeyFrame *add_keyframe(KeyFrameTime t, const T &value)
    {
        KeyFrame *added_frame = nullptr;
        std::unique_ptr<KeyFrameTyped<T>> keyframe;
        if (m_keyframes.empty()) {
            keyframe = std::make_unique<KeyFrameTyped<T>>(this, value, t, false, Vec2D(), Vec2D());
        } else {
            assertm(m_animators.size(), k_no_animator_message);
            keyframe = std::make_unique<KeyFrameTyped<T>>(this, value, t, false, Vec2D(), Vec2D());
        }
        if (add_keyframe(keyframe.get())) {
            added_frame = keyframe.release();
        }
        return added_frame;
    }

    bool add_keyframe(KeyFrame *frame) override
    {
        bool already_exist = false;
        bool added = false;
        for (KeyFrame *f : m_keyframes) {
            if (f == frame || f->time() == frame->time()) {
                already_exist = true;
                break;
            }
        }

        if (!already_exist) {
            int index = 0;
            for (KeyFrame *f : m_keyframes) {
                if (f->time() > frame->time()) {
                    m_keyframes.insert(m_keyframes.begin() + index, frame);
                    added = true;
                    break;
                }
                ++index;
            }

            if (!added) {
                bool empty_before = m_keyframes.empty();
                m_keyframes.emplace_back(frame);
                if (empty_before) {
                    m_animation_type = PropertyAnimationType::e_KeyFramed;
                }
                added = true;
            }
        }

        if (added) {
            notify();
        }

        return added;
    }

    void remove_keyframe(KeyFrame *frame) override
    {
        m_keyframes.erase(std::remove(m_keyframes.begin(), m_keyframes.end(), frame),
                          m_keyframes.end());
        if (m_keyframes.empty()) {
            m_animation_type = PropertyAnimationType::e_Constant;
        }
        notify();
    }

    KeyFrame *remove_keyframe(KeyFrameTime t) override
    {
        KeyFrame *removed = nullptr;
        m_keyframes.erase(std::remove_if(m_keyframes.begin(),
                                         m_keyframes.end(),
                                         [&](KeyFrame *frame) {
                                             if (frame->time() == t) {
                                                 removed = frame;
                                                 return true;
                                             }
                                             return false;
                                         }),
                          m_keyframes.end());
        if (m_keyframes.empty()) {
            m_animation_type = PropertyAnimationType::e_Constant;
        }
        notify();
        return removed;
    }

    int keyframes_count() const override { return m_keyframes.size(); }

    KeyFrameStatus keyframe_status(KeyFrameTime time) const override
    {
        auto is_scripted = [](const KeyFrame *keyframe) {
            return keyframe->expression_enabled() && !keyframe->expression().empty();
        };

        KeyFrameStatus status;
        if (m_animation_type == PropertyAnimationType::e_Constant)
            return status;

        size_t i = 0;
        if (m_keyframes.size() == 1) {
            const auto &keyframe = m_keyframes[0];
            if (time > keyframe->time()) {
                status.on_left = true;
                status.previous = keyframe->time();
                status.left_scripted = is_scripted(keyframe);
            } else if (time < keyframe->time()) {
                status.on_right = true;
                status.next = keyframe->time();
                status.right_scripted = is_scripted(keyframe);
            } else {
                status.at_keyframe = true;
                status.scripted = is_scripted(keyframe);
                status.current = time;
            }
        } else {
            const auto last_index = (m_keyframes.size() - 1);
            const auto second_last_index = last_index - 1;
            for (; i < last_index; ++i) {
                const auto &keyframe = m_keyframes[i];
                const auto &next_keyframe = m_keyframes[i + 1];
                if (keyframe->time() == time) {
                    status.at_keyframe = true;
                    status.scripted = is_scripted(keyframe);
                    status.current = time;
                    status.next = next_keyframe->time();
                    status.right_scripted = is_scripted(next_keyframe);
                    status.on_right = true;
                    if (i > 0) {
                        status.on_left = true;
                        status.previous = m_keyframes[i - 1]->time();
                        status.left_scripted = is_scripted(m_keyframes[i - 1]);
                    }
                    break;
                } else if (next_keyframe->time() == time) {
                    status.at_keyframe = true;
                    status.scripted = is_scripted(next_keyframe);
                    status.previous = keyframe->time();
                    status.on_left = true;
                    status.left_scripted = is_scripted(keyframe);
                    if (i < second_last_index) {
                        status.next = m_keyframes[i + 2]->time();
                        status.right_scripted = is_scripted(m_keyframes[i + 2]);
                        status.on_right = true;
                    }
                    break;
                } else if (keyframe->time() < time && next_keyframe->time() > time) {
                    status.on_left = true;
                    status.on_right = true;
                    status.next = next_keyframe->time();
                    status.previous = keyframe->time();
                    status.left_scripted = is_scripted(keyframe);
                    status.right_scripted = is_scripted(next_keyframe);
                    break;
                } else if (time < keyframe->time()) {
                    status.on_right = true;
                    status.next = keyframe->time();
                    status.right_scripted = is_scripted(keyframe);
                    break;
                }
            }
            if (i >= last_index) {
                status.on_left = true;
                status.previous = m_keyframes[last_index]->time();
                status.left_scripted = is_scripted(m_keyframes[last_index]);
            }
        }

        return status;
    }

    KeyFrameTime next_keyframe_time(KeyFrameTime time) const override
    {
        assert(m_animation_type == PropertyAnimationType::e_KeyFramed && m_keyframes.size() > 1);
        KeyFrameTime next_keyframe_time = time;
        auto last_index = (m_keyframes.size() - 1);
        for (size_t i = 0; i < last_index; ++i) {
            const auto &keyframe = m_keyframes[i];
            const auto &next_keyframe = m_keyframes[i + 1];
            if (keyframe->time() <= time && next_keyframe->time() > time) {
                next_keyframe_time = next_keyframe->time();
            }
        }

        return next_keyframe_time;
    }

    KeyFrameTime previous_keyframe_time(KeyFrameTime time) const override
    {
        assert(m_animation_type == PropertyAnimationType::e_KeyFramed && m_keyframes.size() > 1);
        KeyFrameTime previous_keyframe_time = time;
        auto last_index = (m_keyframes.size() - 1);
        for (size_t i = 0; i < last_index; ++i) {
            const auto &keyframe = m_keyframes[i];
            const auto &next_keyframe = m_keyframes[i + 1];
            if (keyframe->time() < time && next_keyframe->time() >= time) {
                previous_keyframe_time = keyframe->time();
            }
        }

        return previous_keyframe_time;
    }

protected:
    T m_value = {};
    KeyFrames m_keyframes;
    std::string m_expression;
    PropertyAnimationType m_animation_type = PropertyAnimationType::e_Constant;
    mutable std::vector<PropertyAnimatorPtr<T>> m_animators;
    const PropertyValueValidator<T> *m_validator = nullptr;
};

} // namespace inae::model

#endif // DYNAMIC_PROPERTY_H
