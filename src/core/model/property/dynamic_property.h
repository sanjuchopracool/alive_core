#ifndef DYNAMIC_PROPERTY_H
#define DYNAMIC_PROPERTY_H

#include "property.h"
#include "property_animator.h"
#include "property_template.h"
#include "property_value_validator.h"
#include <algorithm>
#include <core/logging/logger.h>
#include <core/math_utility.h>
#include <core/model/keyframes/keyframe.h>
#include <memory>
#include <type_traits>

namespace alive::model {
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
                    const PropertyValueValidator<T> *validator = nullptr,
                    const std::string name = {})
        : PropertyTemplate<T>(object, descriptor, name)
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
        if (m_animation_type == PropertyAnimationType::e_Constant
            && (m_expression.empty() || !m_enable_expression)) {
            return m_value;
        } else if (m_animation_type == PropertyAnimationType::e_KeyFramed) {
            assertm(m_animators.size(), k_no_animator_message);
            return m_animators[0]->value();
        } else {
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

    // return keyframe only when new keyframe is added
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
                keyframe = nullptr;
            }
        } else {
            m_expression = expr;
            m_enable_expression = true;
            notify();
        }

        return keyframe;
    }

    void mark_dirty() override {
        notify();
    }

    KeyFrame *set_expression(const std::string &script, KeyFrameTime time) override
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

    bool is_animating() const override
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

    KeyFrames remove_all_keyframes() override
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

    bool sort_keyframes_by_time() override
    {
        if (std::adjacent_find(m_keyframes.cbegin(),
                               m_keyframes.cend(),
                               [](const KeyFrame *t1, const KeyFrame *t2) {
                                   return t1->time() > t2->time();
                               })
            != m_keyframes.end()) {
            sort(m_keyframes.begin(),
                 m_keyframes.end(),
                 [](const KeyFrame *t1, const KeyFrame *t2) { return t1->time() < t2->time(); });
            notify();
            return true;
        }
        return false;
    }

    KeyFrame *add_keyframe(KeyFrameTime t, const T &value)
    {
        KeyFrame *added_frame = nullptr;
        std::unique_ptr<KeyFrameTyped<T>> keyframe;
        if (m_keyframes.empty()) {
            keyframe = std::make_unique<KeyFrameTyped<T>>(this,
                                                          value,
                                                          t,
                                                          false,
                                                          Vec2D{0.25, 0.25},
                                                          Vec2D{0.75, 0.75});
        } else {
            assertm(m_animators.size(), k_no_animator_message);
            keyframe = std::make_unique<KeyFrameTyped<T>>(this,
                                                          value,
                                                          t,
                                                          false,
                                                          Vec2D{0.25, 0.25},
                                                          Vec2D{0.75, 0.75});
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

    KeyFrame *keyframe_at(KeyFrameTime t) override
    {
        KeyFrame *found = nullptr;
        for (auto *keyframe : m_keyframes) {
            if (keyframe->time() == t) {
                found = keyframe;
                break;
            }
        }
        return found;
    }

    bool update_keyframe_time(KeyFrame *keyframe, KeyFrameTime t) override
    {
        auto result = false;
        if (keyframe && t != keyframe->time()) {
            if (std::find(m_keyframes.begin(), m_keyframes.end(), keyframe) != m_keyframes.end()) {
                keyframe->set_time(t);
                result = true;

                // check if need to sort
                if (!sort_keyframes_by_time()) {
                    notify();
                }
            }
        }
        return result;
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

    DynamicProperty(Object *object, const DynamicProperty &other)
        : PropertyTemplate<T>(object, other.m_descriptor, other.name())
        , m_value(other.m_value)
        , m_expression(other.m_expression)
        , m_animation_type(other.m_animation_type)
        , m_validator(other.m_validator)
    {
        for (const auto item : other.m_keyframes) {
            m_keyframes.emplace_back(item->clone(this));
        }
    }

    DynamicProperty *clone(Object *object) const override
    {
        return new DynamicProperty(object, *this);
    }

    virtual void update_linked_value(const T & /*old_value*/, T & /*new_value*/) {}

    void enable_expression(bool enable) override
    {
        m_enable_expression = enable;
        notify();
    }
    bool expression_enabled() const override { return m_enable_expression; }

protected:
    T m_value = {};
    KeyFrames m_keyframes;
    std::string m_expression;
    bool m_enable_expression = false;
    PropertyAnimationType m_animation_type = PropertyAnimationType::e_Constant;
    mutable std::vector<PropertyAnimatorPtr<T>> m_animators;
    const PropertyValueValidator<T> *m_validator = nullptr;
};

template<typename T>
inline constexpr bool k_is_vec2_d = std::is_same_v<T, Vec2D>;

template<typename T>
inline constexpr bool k_is_vec3_d = std::is_same_v<T, Vec3D>;

template<typename T, typename = std::enable_if_t<k_is_vec2_d<T> || k_is_vec3_d<T>>>
class LinkedDynamicProperty : public DynamicProperty<T>
{
public:
    LinkedDynamicProperty(Object *object,
                          const PropertyDescriptor &descriptor,
                          const T &default_value = {},
                          const PropertyValueValidator<T> *validator = nullptr)
        : DynamicProperty<T>(object, descriptor, default_value, validator)
    {}

    LinkedDynamicProperty(Object *object, const LinkedDynamicProperty &other)
        : DynamicProperty<T>(object, other)
        , m_linked(other.m_linked)
        , m_linked_ratio(other.m_linked_ratio)
    {}

    // these are editor specific
    bool is_linked() const override { return m_linked; }
    bool set_linked(bool link) override
    {
        if (link && link != m_linked) {
            if (math::zero(DynamicProperty<T>::m_value.y()))
                return false;

            m_linked_ratio = DynamicProperty<T>::m_value.x() / DynamicProperty<T>::m_value.y();
        }
        m_linked = link;
        return true;
    }

    LinkedDynamicProperty *clone(Object *object) const override
    {
        return new LinkedDynamicProperty(object, *this);
    }

    void update_linked_value(const T &old_value, T &new_value) override
    {
        if (m_linked && new_value != old_value) {
            if (old_value.x() != new_value.x()) {
                new_value.y() = new_value.x() / m_linked_ratio;
            } else if (old_value.y() != new_value.y()) {
                new_value.x() = new_value.y() * m_linked_ratio;
            }
        }
    }

private:
    bool m_linked = true;
    float m_linked_ratio = 1.0;
};
} // namespace alive::model

#endif // DYNAMIC_PROPERTY_H
