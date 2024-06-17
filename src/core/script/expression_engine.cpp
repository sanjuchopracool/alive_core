#include "expression_engine.h"
#include <core/composition/animation_nodes/transformation_node.h>
#include <core/composition/composition_node.h>
#include <core/composition/solid_layer_node.h>
#include <core/composition/text_layer_node.h>
#include <core/model/text_primitives/text_document_property.h>
namespace inae::script {

void register_types(sol::state &lua)
{
    using namespace inae::model;

    // Vec3D
    sol::usertype<Vec3D> vec3_typ = lua.new_usertype<Vec3D>("Vec3D", sol::constructors<Vec3D()>());
    vec3_typ["create"] = &create_vec3d;
    vec3_typ["x"] = &Vec3D::x;
    // vec3_typ["y"] = &Vec3D::y;
    // vec3_typ["z"] = &Vec3D::z;

    // Vec2D
    // sol::usertype<Vec2D> vec2_typ = lua.new_usertype<Vec2D>("Vec2D");
    // vec2_typ["create"] = &create_vec2d;
    // vec2_typ["x"] = &Vec2D::x;
    // vec2_typ["y"] = &Vec2D::y;

    // Rect
    // sol::usertype<Rect> rect_typ = lua.new_usertype<Rect>("Rect");
    // rect_typ["x"] = sol::readonly_property(&Rect::x);
    // rect_typ["y"] = sol::readonly_property(&Rect::y);
    // rect_typ["left"] = sol::readonly_property(&Rect::left);
    // rect_typ["right"] = sol::readonly_property(&Rect::right);
    // rect_typ["top"] = sol::readonly_property(&Rect::top);
    // rect_typ["bottom"] = sol::readonly_property(&Rect::bottom);
    // rect_typ["width"] = sol::readonly_property(&Rect::width);
    // rect_typ["height"] = sol::readonly_property(&Rect::height);
    // rect_typ["center"] = sol::readonly_property(&Rect::center);
    // rect_typ["center_x"] = sol::readonly_property(&Rect::centerX);
    // rect_typ["center_y"] = sol::readonly_property(&Rect::centerY);

    // // Point
    // sol::usertype<Point> point_typ = lua.new_usertype<Point>("Point");
    // point_typ["x"] = sol::readonly_property(&Point::x);
    // point_typ["y"] = sol::readonly_property(&Point::y);
    // point_typ["is_zero"] = sol::readonly_property(&Point::isZero);
    // point_typ["create"] = &SkPoint::Make;

    // Text
    sol::usertype<core::TextDocumentProperty> text_type
        = lua.new_usertype<core::TextDocumentProperty>("TextDocumentProperty");
    text_type["bbox"] = sol::readonly_property(&core::TextDocumentProperty::bbox);

    sol::usertype<LayerTransformationNode> layer_transform_type
        = lua.new_usertype<LayerTransformationNode>("LayerTransform");
    layer_transform_type["opacity"] = sol::readonly_property(&LayerTransformationNode::opacity);
    layer_transform_type["anchor"] = sol::readonly_property(&LayerTransformationNode::anchor);
    layer_transform_type["scale"] = sol::readonly_property(&LayerTransformationNode::scale);
    layer_transform_type["position"] = sol::readonly_property(&LayerTransformationNode::position);
    layer_transform_type["rotation"] = sol::readonly_property(&LayerTransformationNode::rotation);

    sol::usertype<LayerNode> layer_type = lua.new_usertype<LayerNode>("Layer");
    layer_type["transform"] = sol::readonly_property(&LayerNode::transform_node);

    sol::usertype<SolidLayerNode> solid_layer_type
        = lua.new_usertype<SolidLayerNode>("SolidLayer", sol::base_classes, sol::bases<LayerNode>());
    solid_layer_type["width"] = sol::readonly_property(&SolidLayerNode::width);
    solid_layer_type["height"] = sol::readonly_property(&SolidLayerNode::height);
    solid_layer_type["color"] = sol::readonly_property(&SolidLayerNode::color);

    sol::usertype<TextLayerNode> text_layer_type
        = lua.new_usertype<TextLayerNode>("TextLayer", sol::base_classes, sol::bases<LayerNode>());
    text_layer_type["text"] = sol::readonly_property(&TextLayerNode::text);

    sol::usertype<CompositionNode> comp_type = lua.new_usertype<CompositionNode>("Composition");
    comp_type["layer"] = &CompositionNode::get_layer;
    comp_type["solid_layer"] = &CompositionNode::get_solid_layer;
    comp_type["text_layer"] = &CompositionNode::get_text_layer;
}
ExpressionEngine::ExpressionEngine(const model::CompositionNode &comp)
{
    m_lua.open_libraries(sol::lib::base, sol::lib::package);
    register_types(m_lua);
    m_lua["this_comp"] = std::ref(comp);
}

sol::protected_function_result ExpressionEngine::evaulate(const std::string_view &code)
{
    return m_lua.script(code);
}

sol::protected_function_result ExpressionEngine::evaulate_safely(const std::string_view &code)
{
    return m_lua.script(code, [](lua_State *, sol::protected_function_result pfr) { return pfr; });
}

bool ExpressionEngine::is_valid_expression(std::string_view expression)
{
    auto result = evaulate_safely(expression);
    if (result.valid()) {
        return true;
    }

    return false;
}
} // namespace inae::script
