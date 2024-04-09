#ifndef EXPRESSIONVALUEPROVIDER_H
#define EXPRESSIONVALUEPROVIDER_H

#include "expression_engine.h"
#include <core/logging/logger.h>
#include <core/model/property/dynamic_property.h>
#include <string>

namespace inae::script {

class ExpressionEngine;
template<typename T>
class ExpressionValueProvider
{
public:
    const T &value() const { return m_value; }
    bool evaluate(ExpressionEngine &engine, const std::string &expression)
    {
        auto result = engine.evaulate_safely(expression);
        if (result.valid()) {
            try {
                m_value = result;
                return true;
            } catch (...) {
            }
        }
        INAE_CORE_CRITICAL("expression {} is invalid!", expression);
        return false;
    }
    bool is_valid(ExpressionEngine *engine, const std::string &expression)
    {
        auto result = engine->evaulate_safely(expression);
        if (result.valid()) {
            try {
                m_value = result;
                return true;
            } catch (...) {
            }
        }
        INAE_CORE_CRITICAL("expression {} is invalid!", expression);
        return false;
    }

private:
    T m_value;
};
} // namespace inae::script

#endif // EXPRESSIONVALUEPROVIDER_H
