#ifndef EXPRESSIONENGINE_H
#define EXPRESSIONENGINE_H

#include "expression_engine_interface.h"
#include <core/alive_types/common_types.h>
#include <sol/sol.hpp>
#include <string_view>

namespace alive::model {
class Composition;
class CompositionNode;
class Precomposition;
class PrecompositionLayerNode;
}

namespace alive::script {

class ExpressionEngine : public ExpressionEngineInterface
{
public:
    ExpressionEngine(const model::CompositionNode &comp, InterfaceMode mode = e_CompNode);
    ExpressionEngine(const model::Composition &comp);
    ~ExpressionEngine();

    void reset_comp_node();
    void update_comp(const model::Precomposition *comp);
    void reset_comp();

    void update_pre_comp(const model::PrecompositionLayerNode &comp);
    void update_time(FrameTimeType t);

    const model::CompositionNode *get_comp() const { return m_comp; }
    const model::PrecompositionLayerNode *get_pre_comp() const { return m_pre_comp; }

    sol::protected_function_result evaulate(const std::string_view &code);
    sol::protected_function_result evaulate_safely(const std::string_view &code,
                                                   std::string *error = nullptr);
    std::pair<bool, std::string> is_valid_expression(std::string_view expression) override;

    template<typename T>
    void register_transform_type(T &table);
    template<typename T>
    void register_property_type(T &table);
    template<typename T>
    void register_types_property(sol::state &lua);

private:
    void register_comp_types(sol::state &lua);
private:
    const model::CompositionNode *const m_comp = nullptr;
    InterfaceMode m_mode = e_CompNode;
    sol::state m_lua;
    const model::PrecompositionLayerNode *m_pre_comp = nullptr;
};
} // namespace alive::script
#endif // EXPRESSIONENGINE_H
