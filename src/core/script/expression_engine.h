#ifndef EXPRESSIONENGINE_H
#define EXPRESSIONENGINE_H

#include "expression_engine_interface.h"
#include <sol/sol.hpp>
#include <string_view>

namespace inae::model {
class CompositionNode;
}

namespace inae::script {
class ExpressionEngine : public ExpressionEngineInterface
{
public:
    ExpressionEngine(const model::CompositionNode &comp);

    sol::protected_function_result evaulate(const std::string_view &code);
    sol::protected_function_result evaulate_safely(const std::string_view &code);
    bool is_valid_expression(std::string_view expression) override;

private:
    sol::state m_lua;
};
} // namespace inae::script
#endif // EXPRESSIONENGINE_H
