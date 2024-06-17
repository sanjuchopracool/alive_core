#include "expression_engine.h"
#include <Magnum/Math/Vector3.h>
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
    vec3_typ.set("x",
                 sol::property([](Vec3D &type) { return type.x(); },
                               [](Vec3D &type, float val) { type.x() = val; }));
    vec3_typ.set("y",
                 sol::property([](Vec3D &type) { return type.y(); },
                               [](Vec3D &type, float val) { type.y() = val; }));

    vec3_typ.set("z",
                 sol::property([](Vec3D &type) { return type.z(); },
                               [](Vec3D &type, float val) { type.z() = val; }));

    // Vec2D
    sol::usertype<Vec2D> vec2_typ = lua.new_usertype<Vec2D>("Vec2D");
    vec2_typ["create"] = &create_vec2d;
    vec2_typ.set("x",
                 sol::property([](Vec2D &type) { return type.x(); },
                               [](Vec2D &type, float val) { type.x() = val; }));
    vec2_typ.set("y",
                 sol::property([](Vec2D &type) { return type.y(); },
                               [](Vec2D &type, float val) { type.y() = val; }));

    // Rect
    sol::usertype<SkRect> sk_rect_typ = lua.new_usertype<SkRect>("SkRect");
    sk_rect_typ["x"] = sol::readonly_property(&SkRect::x);
    sk_rect_typ["y"] = sol::readonly_property(&SkRect::y);
    sk_rect_typ["left"] = sol::readonly_property(&SkRect::left);
    sk_rect_typ["right"] = sol::readonly_property(&SkRect::right);
    sk_rect_typ["top"] = sol::readonly_property(&SkRect::top);
    sk_rect_typ["bottom"] = sol::readonly_property(&SkRect::bottom);
    sk_rect_typ["width"] = sol::readonly_property(&SkRect::width);
    sk_rect_typ["height"] = sol::readonly_property(&SkRect::height);
    sk_rect_typ["center"] = sol::readonly_property(&SkRect::center);
    sk_rect_typ["center_x"] = sol::readonly_property(&SkRect::centerX);
    sk_rect_typ["center_y"] = sol::readonly_property(&SkRect::centerY);

    // Point
    sol::usertype<Point> point_typ = lua.new_usertype<Point>("Point");
    point_typ["x"] = sol::readonly_property([](Point &type) { return type.x(); });
    point_typ["y"] = sol::readonly_property([](Point &type) { return type.y(); });
    point_typ["is_zero"] = sol::readonly_property(&Point::isZero);
    vec2_typ["create"] = &create_vec2d;

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
