#ifndef DYNAMIC_PROPERTY_ANIMATOR_H
#define DYNAMIC_PROPERTY_ANIMATOR_H

#include "dynamic_property.h"
#include <core/composition/animation_nodes/animation_node.h>
#include <memory>
#include <core/model/keyframes/keyframe_value_provider.h>
#include <core/script/expression_value_provider.h>

namespace inae::model {

template<typename T>
class DynamicPropertyAnimator : public PropertyAnimator<T>, public CascadeUpdateListener
{
public:
    using CRefFrames = const KeyFrames &;
    using CRefValueType = const T &;
    using CRefPropertyType = const DynamicProperty<T> &;

    DynamicPropertyAnimator(CRefPropertyType property, IObserver *observer)
        : m_property(property)
        , m_kayframe_value_provider(property.get_keyframes())
    {
        observer->observe(this);
        m_property.add_animator(this);
    }

    ~DynamicPropertyAnimator() { m_property.remove_animator(this); }
    void update(UpdateContext &context) override
    {
        if (m_property.value_type() == PropertyAnimationType::e_KeyFramed) {
            if (m_kayframe_value_provider.has_update(context.time)) {
                m_kayframe_value_provider.update(context.time, context.expr_engine);
                notify_observers();
            }
        } else if (!m_property.get_expression(context.time).empty()) {
            m_expression_value_provider.evaluate(context.expr_engine,
                                                 m_property.get_expression(context.time));
        }
    }
    CRefValueType value() const override
    {
        if (m_property.value_type() == PropertyAnimationType::e_KeyFramed) {
            return m_kayframe_value_provider.value();
        }

        if (m_property.get_expression(1).empty()) {
            return m_property.get_value();
        }

        return m_expression_value_provider.value();
    }

    void notify() override { notify_observers(); }

    bool is_valid_expression(script::ExpressionEngineInterface *engine,
                             const std::string &expression) override
    {
        return m_kayframe_value_provider.is_valid_expression(engine, expression);
    }

private:
    CRefPropertyType m_property;
    script::ExpressionValueProvider<T> m_expression_value_provider;
    KeyFrameValueProvider<T> m_kayframe_value_provider;
};

template<typename T>
PropertyAnimatorUniquePtr<T> create_animator(const DynamicProperty<T> &property,
                                             IObserver *listener)
{
    return std::make_unique<DynamicPropertyAnimator<T>>(property, listener);
}

} // namespace inae::model

#endif // DYNAMIC_PROPERTY_ANIMATOR_H
