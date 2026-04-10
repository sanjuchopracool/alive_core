#include "expression_engine.h"
#include "helper.h"
#include <Magnum/Math/Vector3.h>
#include <boost/algorithm/string.hpp>
#include <core/alive_types/color_type.h>
#include <core/composition/animation_nodes/rectangle_node.h>
#include <core/composition/animation_nodes/transformation_node.h>
#include <core/composition/composition_node.h>
#include <core/composition/precomposition_layer_node.h>
#include <core/composition/shape_layer_node.h>
#include <core/composition/solid_layer_node.h>
#include <core/composition/text_layer_node.h>
#include <core/model/composition.h>
#include <core/model/layers/layer.h>
#include <core/model/layers/null_layer.h>
#include <core/model/layers/shape_layer.h>
#include <core/model/layers/text_layer.h>
#include <core/model/property/dynamic_property.h>
#include <core/model/property/property.h>
#include <core/model/property/property_group.h>
#include <core/model/property/property_template.h>
#include <core/model/property/static_property.h>
#include <core/model/shape_items/fill.h>
#include <core/model/shape_items/rectangle.h>
#include <core/model/shape_items/stroke.h>
#include <core/model/text_primitives/text.h>
#include <core/model/transform.h>
#include <core/model/tree_item.h>
#include <core/tagging/tag.h>
#include <core/tagging/taggable.h>
#include <core/tagging/tag_manager.h>
#include <iostream>

namespace {
constexpr char k_this[] = "this";
constexpr char k_node_suffix[] = "_node";
constexpr char k_layer[] = "layer";
constexpr char k_properties[] = "properties";
constexpr char k_solid_layer[] = "solid_layer";
constexpr char k_transform[] = "transform";
constexpr char k_text_layer[] = "text_layer";
constexpr char k_shape_layer[] = "shape_layer";
constexpr char k_null_layer[] = "null_layer";
constexpr char k_get_keyframe[] = "get_keyframe";

// transform
constexpr char k_opacity[] = "opacity";
constexpr char k_width[] = "width";
constexpr char k_height[] = "height";
constexpr char k_anchor[] = "anchor";
constexpr char k_scale[] = "scale";
constexpr char k_position[] = "position";
constexpr char k_rotation[] = "rotation";
constexpr char k_color[] = "color";
constexpr char k_size[] = "size";
constexpr char k_roundness[] = "roundness";
constexpr char k_visible[] = "visible";

constexpr char k_rect[] = "rect";
constexpr char k_fill[] = "fill";
constexpr char k_stroke[] = "stroke";
constexpr char k_name[] = "name";
constexpr char k_id[] = "id";
constexpr char k_count[] = "k_count";
constexpr char k_at[] = "k_at";
constexpr char k_has_tag[] = "k_has_tag";

std::string suffixed(bool suffix, const char *data)
{
    auto res = std::string(data);
    return suffix ? (res + k_node_suffix) : res;
}

} // namespace
namespace alive::script {

inline Vec3D create_vec3d(Vec1D x = 0.f, Vec1D y = 0.f, Vec1D z = 0.f)
{
    return Vec3D{x, y, z};
}

inline Vec2D create_vec2d(Vec1D x = 0.f, Vec1D y = 0.0f)
{
    return Vec2D{x, y};
}

using namespace alive::model;
template<typename T>
void register_item_type(T &table)
{
    table[k_name] = sol::property(&alive::TreeItem::name, &alive::TreeItem::set_name);
}

void register_tag(sol::state &lua)
{
    sol::usertype<alive::Tag> table = lua.new_usertype<alive::Tag>("Tag");
    table[k_name] = sol::readonly_property(&Tag::name);
    table[k_id] = sol::readonly_property(&Tag::id);
    table[k_at] = sol::readonly_property(&Tag::at);
    table[k_count] = sol::readonly_property(&Tag::count);
}

void register_taggable(sol::state &lua)
{
    sol::usertype<alive::Taggable> table = lua.new_usertype<alive::Taggable>("Taggable");
    table[k_name] = sol::readonly_property(&Taggable::name);
    table[k_has_tag] = sol::readonly_property(&Taggable::has_tag);
}

void register_tag_manager(sol::state &lua)
{

}

template<typename T>
void register_keyframe_type(T &table)
{
    table["time"] = sol::readonly_property(&KeyFrame::time);
}

template<typename T>
decltype(auto) get_static_sol_property()
{
    auto getter = [](Property *p) { return static_cast<PropertyTemplate<T> *>(p)->get_value(); };

    auto setter = [](Property *p, const T &val) {
            static_cast<PropertyTemplate<T> *>(p)->set_value(val);
    };
    return sol::property([getter](Property *p) { return getter(p); },
                         [setter](Property *p, const T &val) { setter(p, val); });
}

template<typename T>
void ExpressionEngine::register_property_type(T &table)
{
    register_item_type(table);
    table["can_animate"] = sol::readonly_property(&alive::model::Property::can_animate);
    table["is_animating"] = sol::readonly_property(&alive::model::Property::is_animating);
    table["is_animating"] = sol::readonly_property(&alive::model::Property::is_animating);
    table["keyframe_at"] = &alive::model::Property::keyframe_at;
    table["is_valid_expression"] = [this](alive::model::Property *p, const std::string &expr) {
        auto str = expr;
        boost::trim(str);
        if (expr.empty()) {
            return false;
        }
        return p->is_valid_expression(this, str);
    };
    table["update_keyframe_time"] = &alive::model::Property::update_keyframe_time;
    table["sort_keyframes"] = &alive::model::Property::sort_keyframes_by_time;

    table["to_string"] = get_static_sol_property<std::string>();
    table["to_color3v"] = get_static_sol_property<Color>();
    table["to_float"] = get_static_sol_property<float>();
    table["to_color4ub"] = get_static_sol_property<Color4ub>();
    table["to_font"] = get_static_sol_property<core::Font>();
    table["to_bool"] = get_static_sol_property<bool>();
    table["to_vec3d"] = get_static_sol_property<Vec3D>();
    table["to_int"] = get_static_sol_property<int>();
}

template<typename T>
void register_group_item_type(T &table)
{
    register_item_type(table);
}

template<typename T>
void register_object_type(T &table)
{
    register_group_item_type(table);
    table[k_visible] = sol::property(&Object::is_object_visible, &Object::set_object_visible);
}

template<typename T>
void register_property_group_type(T &table)
{
    register_object_type(table);
    table["count"] = sol::readonly_property(&PropertyGroup::count);
    table["search"] = sol::resolve<Property *(const std::string &)>(
        &PropertyGroup::search_with_name);
    table["search_by_id"] = sol::resolve<Property *(const std::string &)>(
        &PropertyGroup::search_with_id);
    table["at_index"] = sol::resolve<Property *(size_t)>(&PropertyGroup::at);
    table["add"] = &PropertyGroup::add;
    table["delete"] = sol::resolve<int(Property *)>(&PropertyGroup::remove);
    table["delete_by_index"] = sol::resolve<Property *(size_t)>(&PropertyGroup::remove);
    table["delete_by_name"] = sol::resolve<Property *(const std::string &)>(&PropertyGroup::remove);
}

template<typename T>
void ExpressionEngine::register_transform_type(T &table)
{
    register_object_type(table);
    table[k_opacity] = sol::readonly_property(
        [](model::Transform &transform) { return &transform.opacity(); });
    table[k_anchor] = sol::readonly_property(
        [](model::Transform &transform) { return &transform.anchor(); });
    table[k_scale] = sol::readonly_property(
        [](model::Transform &transform) { return &transform.scale(); });
    table[k_position] = sol::readonly_property(
        [](model::Transform &transform) { return &transform.position(); });
    table[k_rotation] = sol::readonly_property(
        [](model::Transform &transform) { return &transform.rotation(); });
}

template<typename T>
void register_layer_type(T &table)
{
    register_object_type(table);
    table[k_transform] = sol::readonly_property(
        [](const Layer &layer) { return &layer.transform(); });
    table[k_properties] = sol::readonly_property(&Layer::property_group);
}

template<typename T>
void register_precomp_type(T &table)
{
    register_object_type(table);
    table[k_layer] = &Precomposition::search_layer;
    table[k_text_layer] = &Precomposition::search_text_layer;
    table[k_shape_layer] = &Precomposition::search_shape_layer;
    table[k_null_layer] = &Precomposition::search_null_layer;
    table[k_get_keyframe] = &Precomposition::get_keyframe;
    table[k_width] = sol::readonly_property(&Precomposition::width);
    table[k_height] = sol::readonly_property(&Precomposition::height);
    table[k_properties] = sol::readonly_property(
        sol::resolve<PropertyGroup *()>(&Precomposition::property_group));
    table["apply_script"] = sol::property(&Precomposition::apply_script,
                                          &Precomposition::set_apply_script);
}

template<typename T>
void register_comp_type(T &table)
{
    register_precomp_type(table);
}

template<typename T>
void register_shape_type(T &table)
{
    register_object_type(table);
    table["type"] = sol::property(
        [](const model::ShapeItem &item) { return static_cast<int>(item.type()); });
}

void register_shape_items(sol::state &lua)
{
    // Join Style
    lua["ShapeType"] = lua.create_table_with("Rectangle",
                                             ShapeType::e_Rectangle,
                                             "Fill",
                                             ShapeType::e_Fill,
                                             "Stroke",
                                             ShapeType::e_Stroke);

    sol::usertype<model::ShapeItem> shape_item_type
        = lua.new_usertype<model::ShapeItem>("ShapeItem",
                                             sol::base_classes,
                                             sol::bases<alive::model::Object>());
    register_shape_type(shape_item_type);

    sol::usertype<model::Rectangle> rect_item_type
        = lua.new_usertype<model::Rectangle>("Rectangle",
                                             sol::base_classes,
                                             sol::bases<alive::model::ShapeItem>());
    rect_item_type[k_size] = sol::readonly_property(
        [](model::Rectangle &rect) { return &rect.size(); });
    rect_item_type[k_position] = sol::readonly_property(
        [](model::Rectangle &rect) { return &rect.position(); });
    rect_item_type[k_roundness] = sol::readonly_property(
        [](model::Rectangle &rect) { return &rect.roundness(); });
    register_shape_type(rect_item_type);

    sol::usertype<model::Fill> fill_item_type
        = lua.new_usertype<model::Fill>("Fill",
                                        sol::base_classes,
                                        sol::bases<alive::model::ShapeItem>());
    fill_item_type[k_color] = sol::readonly_property(
        [](model::Fill &fill) { return &fill.color(); });
    fill_item_type[k_opacity] = sol::readonly_property(
        [](model::Fill &fill) { return &fill.opacity(); });
    register_shape_type(fill_item_type);

    sol::usertype<model::Stroke> stroke_item_type
        = lua.new_usertype<model::Stroke>("Stroke",
                                          sol::base_classes,
                                          sol::bases<alive::model::ShapeItem>());
    stroke_item_type[k_color] = sol::readonly_property(
        [](model::Stroke &stroke) { return &stroke.color(); });
    stroke_item_type[k_opacity] = sol::readonly_property(
        [](model::Stroke &stroke) { return &stroke.opacity(); });
    stroke_item_type[k_width] = sol::readonly_property(
        [](model::Stroke &stroke) { return &stroke.width(); });
    register_shape_type(stroke_item_type);
}

template<typename T>
void ExpressionEngine::register_types_property(sol::state &lua)
{
    std::string type_name;
    if constexpr (std::is_same_v<int, T>) {
        type_name = "int";
    } else if constexpr (std::is_same_v<Color, T>) {
        type_name = "Color";
    } else if constexpr (std::is_same_v<float, T>) {
        type_name = "float";
    } else if constexpr (std::is_same_v<Vec2D, T>) {
        type_name = "Vec2D";
    } else if constexpr (std::is_same_v<Vec3D, T>) {
        type_name = "Vec3D";
    }

    assertm(!type_name.empty(), "Unknown typename should not be empty");
    {
        sol::usertype<alive::model::DynamicProperty<T>> property_item = lua.new_usertype<
            alive::model::DynamicProperty<T>>(std::string("DProperty") + type_name,
                                              sol::base_classes,
                                              sol::bases<alive::model::Property>());
        register_property_type(property_item);
        property_item["value"] = sol::property(&alive::model::DynamicProperty<T>::get_value,
                                               [](alive::model::DynamicProperty<T> &p,
                                                  const T &val) { p.set_value(val); });
        property_item["set_value_at"] = [](alive::model::DynamicProperty<T> &p,
                                           const T &val,
                                           KeyFrameTime t) { p.set_value(val, t); };
    }
}

void ExpressionEngine::register_comp_types(sol::state &lua)
{
    sol::usertype<alive::TreeItem> item_type = lua.new_usertype<alive::TreeItem>("Item");
    register_item_type(item_type);

    // register tag type
    register_tag(lua);

    // register taggable type
    register_taggable(lua);

    // register Tag Manager
    register_tag_manager(lua);

    sol::usertype<KeyFrame> keyframe_type = lua.new_usertype<KeyFrame>("KeyFrame");
    register_keyframe_type(keyframe_type);

    /*------------------------------------------------------------------------------------
    Property
    ------------------------------------------------------------------------------------*/

    sol::usertype<alive::model::Property> property_item
        = lua.new_usertype<alive::model::Property>("Property",
                                                   sol::base_classes,
                                                   sol::bases<alive::TreeItem>());
    register_property_type(property_item);
    /*------------------------------------------------------------------------------------
    Dynamic Property
    ------------------------------------------------------------------------------------*/
    register_types_property<Color>(lua);
    register_types_property<float>(lua);
    register_types_property<Size2D>(lua);
    register_types_property<Vec3D>(lua);

    sol::usertype<alive::GroupTreeItem> group_item_type
        = lua.new_usertype<alive::GroupTreeItem>("GroupItem",
                                                 sol::base_classes,
                                                 sol::bases<alive::TreeItem>());
    register_group_item_type(group_item_type);

    sol::usertype<model::Object> object_item_type
        = lua.new_usertype<model::Object>("Object",
                                          sol::base_classes,
                                          sol::bases<alive::GroupTreeItem>());
    register_object_type(object_item_type);
    /*------------------------------------------------------------------------------------
     Shape Items
    ------------------------------------------------------------------------------------*/
    register_shape_items(lua);

    /*------------------------------------------------------------------------------------
    Layer
    ------------------------------------------------------------------------------------*/
    {
        sol::usertype<Transform> transform_type = lua.new_usertype<Transform>(k_transform,
                                                                              sol::base_classes,
                                                                              sol::bases<Object>());
        register_transform_type(transform_type);
    }

    {
        sol::usertype<Layer> layer_type = lua.new_usertype<Layer>("Layer",
                                                                  sol::base_classes,
                                                                  sol::bases<Object>());
        register_layer_type(layer_type);
    }
    {
        sol::usertype<TextLayer> text_layer_type = lua.new_usertype<TextLayer>("TextLayer",
                                                                               sol::base_classes,
                                                                               sol::bases<Layer>());
        register_layer_type(text_layer_type);
        text_layer_type["source_text"]
            = sol::property([](const TextLayer &layer) { return layer.get_text(); },
                            [](TextLayer &layer, const core::Text &text) { layer.set_text(text); });
    }

    {
        sol::usertype<ShapeLayer> shape_layer_type
            = lua.new_usertype<ShapeLayer>("ShapeLayer", sol::base_classes, sol::bases<Layer>());
        register_layer_type(shape_layer_type);
        shape_layer_type[k_rect] = &ShapeLayer::search_rect;
        shape_layer_type[k_fill] = &ShapeLayer::search_fill;
        shape_layer_type[k_stroke] = &ShapeLayer::search_stroke;
    }

    {
        sol::usertype<NullLayer> null_layer_type = lua.new_usertype<NullLayer>("NullLayer",
                                                                               sol::base_classes,
                                                                               sol::bases<Layer>());
        register_layer_type(null_layer_type);
    }

    /*------------------------------------------------------------------------------------
    Composition
    ------------------------------------------------------------------------------------*/

    sol::usertype<PropertyGroup> prop_group_type
        = lua.new_usertype<PropertyGroup>("PropertyGroup", sol::base_classes, sol::bases<Object>());
    register_property_group_type(prop_group_type);

    sol::usertype<Precomposition> precomposition_item_type
        = lua.new_usertype<Precomposition>("Precomposition",
                                           sol::base_classes,
                                           sol::bases<Object>());
    register_precomp_type(precomposition_item_type);

    sol::usertype<Composition> composition_item_type
        = lua.new_usertype<Composition>("Composition",
                                        sol::base_classes,
                                        sol::bases<Precomposition>());
    register_comp_type(composition_item_type);
}

void register_comp_node_types(sol::state &lua, bool suffix = false)
{
    /*------------------------------------------------------------------------------------
    Layer
    ------------------------------------------------------------------------------------*/
    sol::usertype<LayerTransformationNode> layer_transform_type
        = lua.new_usertype<LayerTransformationNode>("LayerTransformNode");
    layer_transform_type[k_opacity] = sol::readonly_property(&LayerTransformationNode::opacity);
    layer_transform_type[k_anchor] = sol::readonly_property(&LayerTransformationNode::anchor);
    layer_transform_type[k_scale] = sol::readonly_property(&LayerTransformationNode::scale);
    layer_transform_type[k_position] = sol::readonly_property(&LayerTransformationNode::position);
    layer_transform_type[k_rotation] = sol::readonly_property(&LayerTransformationNode::rotation);

    sol::usertype<LayerNode> layer_type = lua.new_usertype<LayerNode>("LayerNode");
    layer_type[k_transform] = sol::readonly_property(&LayerNode::transform_node);

    sol::usertype<SolidLayerNode> solid_layer_type
        = lua.new_usertype<SolidLayerNode>("SolidLayerNode",
                                           sol::base_classes,
                                           sol::bases<LayerNode>());
    solid_layer_type["width"] = sol::readonly_property(&SolidLayerNode::width);
    solid_layer_type["height"] = sol::readonly_property(&SolidLayerNode::height);
    solid_layer_type["color"] = sol::readonly_property(&SolidLayerNode::color);

    sol::usertype<TextLayerNode> text_layer_type
        = lua.new_usertype<TextLayerNode>("TextLayerNode",
                                          sol::base_classes,
                                          sol::bases<LayerNode>());
    text_layer_type["text"] = sol::readonly_property(&TextLayerNode::text);

    {
        // register rect
        sol::usertype<model::RectangleNode> rect_item_type = lua.new_usertype<model::RectangleNode>(
            "RectangleNode");
        rect_item_type[k_size] = sol::readonly_property(&RectangleNode::size);

        sol::usertype<ShapeLayerNode> shape_layer_node
            = lua.new_usertype<ShapeLayerNode>("ShapeLayerNode",
                                               sol::base_classes,
                                               sol::bases<LayerNode>());
        // shape_layer_node[k_rect] = &shape_layer_node::search_rect;
    }

    sol::usertype<LayerNodesContainer> layer_nodes_container
        = lua.new_usertype<LayerNodesContainer>("LayerNodesContainer");
    layer_nodes_container[k_layer] = &LayerNodesContainer::get_layer;
    layer_nodes_container[k_solid_layer] = &LayerNodesContainer::get_solid_layer;
    layer_nodes_container[k_text_layer] = &LayerNodesContainer::get_text_layer;
    layer_nodes_container[k_shape_layer] = &LayerNodesContainer::get_shape_layer;

    sol::usertype<PrecompositionLayerNode> precomp_layer_type
        = lua.new_usertype<PrecompositionLayerNode>("PrecompositionLayerNode",
                                                    sol::base_classes,
                                                    sol::bases<LayerNodesContainer>());

    sol::usertype<CompositionNode> comp_type
        = lua.new_usertype<CompositionNode>("CompositionNode",
                                            sol::base_classes,
                                            sol::bases<LayerNodesContainer>());
}
void register_types(sol::state &lua)
{
    /*------------------------------------------------------------------------------------
    // Global Function  
    ------------------------------------------------------------------------------------*/
    // color type
    register_color_types(lua);

    /*------------------------------------------------------------------------------------
    // Vec2D used as Point2D and Size2D    
    ------------------------------------------------------------------------------------*/
    sol::usertype<Vec2D> vec2_typ = lua.new_usertype<Vec2D>("Vec2D");
    vec2_typ["new"] = &create_vec2d;
    vec2_typ.set("x",
                 sol::property([](Vec2D &type) { return type.x(); },
                               [](Vec2D &type, Vec1D val) { type.x() = val; }));
    vec2_typ.set("y",
                 sol::property([](Vec2D &type) { return type.y(); },
                               [](Vec2D &type, Vec1D val) { type.y() = val; }));
    vec2_typ["width"] = sol::resolve<Vec1D &()>(&Vec2D::x);
    vec2_typ["height"] = sol::resolve<Vec1D &()>(&Vec2D::y);
    vec2_typ["is_zero"] = sol::readonly_property(&Vec2D::isZero);
    vec2_typ["is_normalized"] = sol::readonly_property(&Vec2D::isNormalized);

    /*------------------------------------------------------------------------------------
    // Vec3D used as Point3D and Size3D    
    ------------------------------------------------------------------------------------*/
    sol::usertype<Vec3D> vec3_typ = lua.new_usertype<Vec3D>("Vec3D", sol::constructors<Vec3D()>());
    vec3_typ["new"] = &create_vec3d;
    vec3_typ.set("x",
                 sol::property([](Vec3D &type) { return type.x(); },
                               [](Vec3D &type, Vec1D val) { type.x() = val; }));
    vec3_typ.set("y",
                 sol::property([](Vec3D &type) { return type.y(); },
                               [](Vec3D &type, Vec1D val) { type.y() = val; }));
    vec3_typ.set("z",
                 sol::property([](Vec3D &type) { return type.z(); },
                               [](Vec3D &type, Vec1D val) { type.z() = val; }));
    vec3_typ["is_zero"] = sol::readonly_property(&Vec3D::isZero);
    vec3_typ["is_normalized"] = sol::readonly_property(&Vec3D::isNormalized);

    // Rect2D
    sol::usertype<Rect2D> rect_typ = lua.new_usertype<Rect2D>("Rect2D");
    // sk_rect_typ["x"] = sol::readonly_property(&SkRect::x);
    // sk_rect_typ["y"] = sol::readonly_property(&SkRect::y);
    rect_typ["left"] = sol::readonly_property([](Rect2D &rect) { return rect.left(); });
    rect_typ["right"] = sol::readonly_property([](Rect2D &rect) { return rect.right(); });
    rect_typ["top"] = sol::readonly_property([](Rect2D &rect) { return rect.top(); });
    rect_typ["bottom"] = sol::readonly_property([](Rect2D &rect) { return rect.bottom(); });
    rect_typ["width"] = sol::readonly_property([](Rect2D &rect) { return rect.sizeX(); });
    rect_typ["height"] = sol::readonly_property([](Rect2D &rect) { return rect.sizeY(); });
    rect_typ["center"] = sol::readonly_property([](Rect2D &rect) { return rect.center(); });
    rect_typ["center_x"] = sol::readonly_property([](Rect2D &rect) { return rect.centerX(); });
    rect_typ["center_y"] = sol::readonly_property([](Rect2D &rect) { return rect.centerY(); });

    /*------------------------------------------------------------------------------------
    // Vec3D used as Point3D and Size3D    
    ------------------------------------------------------------------------------------*/
    sol::usertype<alive::core::Font> font_typ = lua.new_usertype<alive::core::Font>(
        "AliveFontType");
    font_typ["new"] = &create_vec2d;
    font_typ.set("family",
                 sol::property([](core::Font &type) { return type.family; },
                               [](core::Font &type, int val) { type.family = val; }));
    font_typ.set("style",
                 sol::property([](core::Font &type) { return type.style; },
                               [](core::Font &type, int val) { type.style = val; }));
    font_typ.set("size",
                 sol::property([](core::Font &type) { return type.size; },
                               [](core::Font &type, float val) { type.size = val; }));

    // Point
    // sol::usertype<Point> point_typ = lua.new_usertype<Point>("Point");
    // point_typ["x"] = sol::readonly_property([](Point &type) { return type.x(); });
    // point_typ["y"] = sol::readonly_property([](Point &type) { return type.y(); });
    // point_typ["is_zero"] = sol::readonly_property(&Point::isZero);
    // vec2_typ["create"] = &create_vec2d;

    // Cap Style
    lua["CapStyle"] = lua.create_table_with("Butt",
                                            CapStyle::e_Butt,
                                            "Round",
                                            CapStyle::e_Round,
                                            "Square",
                                            CapStyle::e_Square);
    // Join Style
    lua["JoinStyle"] = lua.create_table_with("Miter",
                                             JoinStyle::e_Miter,
                                             "Round",
                                             JoinStyle::e_Round,
                                             "Bevel",
                                             JoinStyle::e_Bevel);

    // Direction
    lua["Direction"] = lua.create_table_with("Left",
                                             Direction::e_Left,
                                             "Right",
                                             Direction::e_Right,
                                             "Top",
                                             Direction::e_Top,
                                             "Bottom",
                                             Direction::e_Bottom);

    // Text
    sol::usertype<core::Text> text_type = lua.new_usertype<core::Text>("Text");
    text_type["bbox"] = sol::readonly_property(&core::Text::bbox);
    text_type["text"] = sol::property([](const core::Text &text) { return text.m_text; },
                                      [](core::Text &text, const std::string &str) {
                                          text.m_text = str;
                                      });

    text_type["font"] = sol::property([](const core::Text &text) { return text.m_font; },
                                      [](core::Text &text, const core::Font &font) {
                                          text.m_font = font;
                                      });

    text_type[k_fill] = sol::property([](const core::Text &text) { return text.m_fill; },
                                      [](core::Text &text, bool fill) { text.m_fill = fill; });
    text_type["fill_color"] = sol::property([](const core::Text &text) { return text.m_fill_color; },
                                            [](core::Text &text, const Color &color) {
                                                text.m_fill_color = color;
                                            });

    text_type[k_stroke] = sol::property([](const core::Text &text) { return text.m_stroke; },
                                        [](core::Text &text, bool stroke) {
                                            text.m_stroke = stroke;
                                        });
    text_type["stroke_width"]
        = sol::property([](const core::Text &text) { return text.m_stroke_width; },
                        [](core::Text &text, Vec1D size) { text.m_stroke_width = size; });
    text_type["stroke_color"]
        = sol::property([](const core::Text &text) { return text.m_stroke_color; },
                        [](core::Text &text, const Color &color) { text.m_stroke_color = color; });

    text_type["cap_style"]
        = sol::property([](const core::Text &text) { return static_cast<int>(text.m_cap_style); },
                        [](core::Text &text, int style) {
                            text.m_cap_style = static_cast<CapStyle>(style);
                        });
    text_type["join_style"]
        = sol::property([](const core::Text &text) { return static_cast<int>(text.m_join_style); },
                        [](core::Text &text, int style) {
                            text.m_join_style = static_cast<JoinStyle>(style);
                        });
    text_type["update_bbox"] = &core::Text::recalculateBbox;
}

ExpressionEngine::ExpressionEngine(const model::CompositionNode &comp, InterfaceMode mode)
    : m_comp(&comp)
    , m_mode(mode)
{
    m_lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::package);

    register_types(m_lua);
    if (mode == e_CompNode || m_mode == e_Both) {
        register_comp_node_types(m_lua, m_mode == e_Both);
        reset_comp_node();
    }
    if (m_mode == e_Comp || m_mode == e_Both) {
        register_comp_types(m_lua);
        const Composition *an = comp.composition();
        m_lua[k_this] = an;
    }

    Helper::dump_state(m_lua);
}

ExpressionEngine::ExpressionEngine(const model::Composition &comp)
{
    m_lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::package);
    register_types(m_lua);
}

ExpressionEngine::~ExpressionEngine() {}

void ExpressionEngine::reset_comp_node()
{
    m_lua[suffixed(m_mode == e_Both, k_this)] = std::ref(*m_comp);
    update_time(m_comp->time());
}

void ExpressionEngine::update_comp(const Precomposition *comp)
{
    if (m_mode == e_Comp || m_mode == e_Both) {
        m_lua[k_this] = comp;
    }
}

void ExpressionEngine::reset_comp()
{
    if (m_mode == e_Comp || m_mode == e_Both) {
        m_lua[k_this] = m_comp->composition();
    }
}

void ExpressionEngine::update_pre_comp(const model::PrecompositionLayerNode &comp)
{
    m_lua[suffixed(m_mode == e_Both, k_this)] = std::ref(comp);
    m_pre_comp = &comp;
    update_time(m_pre_comp->time());
}

void ExpressionEngine::update_time(FrameTimeType t)
{
    m_lua["frame"] = t;
    m_lua["time"] = t / m_comp->fps();
}

sol::protected_function_result ExpressionEngine::evaulate(const std::string_view &code)
{
    return m_lua.script(code);
}

sol::protected_function_result ExpressionEngine::evaulate_safely(const std::string_view &code,
                                                                 std::string *error)
{
    return m_lua.safe_script(code, [error](lua_State *, sol::protected_function_result pfr) {
        // pfr will contain things that went wrong, for either loading or executing the script
        // the user can do whatever they like here, including throw. Otherwise...
        sol::error err = pfr;
        std::cout << "An error (an expected one) occurred: " << err.what() << std::endl;
        if (error) {
            *error = err.what();
        }
        return pfr;
    });
}

std::pair<bool, std::string> ExpressionEngine::is_valid_expression(std::string_view expression)
{
    std::string error;
    auto result = evaulate_safely(expression, &error);
    if (result.valid()) {
        return {true, error};
    }

    return {false, {error}};
}

ExpressionEngineInterface *create_engine(model::CompositionNode &node,
                                         ExpressionEngineInterface::InterfaceMode mode)
{
    return new ExpressionEngine(node, mode);
}

} // namespace alive::script
