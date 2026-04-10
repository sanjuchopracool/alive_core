#ifndef EXPRESSION_ENGINE_INTERFACE_H
#define EXPRESSION_ENGINE_INTERFACE_H

#include <string>
#include <string_view>

namespace alive {

namespace model {
class CompositionNode;
}
namespace script {
class ExpressionEngineInterface
{
public:
    enum class Exception { e_None, e_Recursive };
    enum InterfaceMode { e_CompNode, e_Comp, e_Both };
    virtual std::pair<bool, std::string> is_valid_expression(std::string_view str) = 0;
    virtual ~ExpressionEngineInterface() {}
};

ExpressionEngineInterface *create_engine(model::CompositionNode &node,
                                         ExpressionEngineInterface::InterfaceMode mode);
} // namespace script
} // namespace alive

#endif // EXPRESSION_ENGINE_INTERFACE_H
