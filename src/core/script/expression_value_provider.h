#ifndef EXPRESSIONVALUEPROVIDER_H
#define EXPRESSIONVALUEPROVIDER_H

#include "expression_engine.h"
#include <core/logging/logger.h>
#include <core/model/property/dynamic_property.h>
#include <core/utility.h>
#include <string>

namespace alive::script {

class ExpressionEngine;
template<typename T>
class ExpressionValueProvider : public model::SimpleSubject
{
public:
    const T &value() const
    {
        if (m_evaluating) {
            m_ex = ExpressionEngine::Exception::e_Recursive;
            throw m_ex;
        }
        return m_value;
    }
    bool evaluate(ExpressionEngine &engine, const std::string &expression)
    {
        if (m_evaluating) {
            throw ExpressionEngine::Exception::e_Recursive;
        }

        m_ex = ExpressionEngine::Exception::e_None;
        alive::BooleanGuard bg(m_evaluating);
        try {
            auto result = engine.evaulate_safely(expression);
            if (result.valid()) {
                T new_val = result;
                if (new_val != m_value) {
                    m_value = result;
                    notify_observers();
                }
                return true;
            } else {
            }
        } catch (...) {
        }

        return false;
    }
    bool is_valid(ExpressionEngine *engine, const std::string &expression)
    {
        if (m_evaluating) {
            throw ExpressionEngine::Exception::e_Recursive;
        }

        m_ex = ExpressionEngine::Exception::e_None;
        alive::BooleanGuard bg(m_evaluating);
        try {
            auto result = engine->evaulate(expression);
            if (result.valid()) {
                m_value = result;
                return true;
            } else {
            }
        } catch (...) {
        }

        throw m_ex;
        return false;
    }

private:
    T m_value;
    bool m_evaluating = false;
    mutable ExpressionEngine::Exception m_ex = ExpressionEngine::Exception::e_None;
};
} // namespace alive::script

#endif // EXPRESSIONVALUEPROVIDER_H
