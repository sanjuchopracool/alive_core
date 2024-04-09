#ifndef EXPRESSION_ENGINE_INTERFACE_H
#define EXPRESSION_ENGINE_INTERFACE_H

#include <string_view>

namespace inae::script {
class ExpressionEngineInterface
{
public:
    virtual bool is_valid_expression(std::string_view str) = 0;
};
} // namespace inae::script
#endif // EXPRESSION_ENGINE_INTERFACE_H
