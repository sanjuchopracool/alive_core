#include "animation_node_factory.h"

#include <core/model/shape_items/ellipse.h>
#include <core/model/shape_items/fill.h>
#include <core/model/shape_items/group.h>
//#include <core/model/shape_items/polystar.h>
#include <core/model/shape_items/rectangle.h>
#include <core/model/shape_items/repeater.h>
#include <core/model/shape_items/shape.h>
#include <core/model/shape_items/shape_transformation.h>
#include <core/model/shape_items/stroke.h>
//#include <core/model/shape_items/trim.h>

#include "ellipse_node.h"
#include "fill_node.h"
#include "group_node.h"
//#include "polystar_node.h"
#include "rectangle_node.h"
#include "repeater_node.h"
#include "shape_node.h"
#include "shape_transformation_node.h"
#include "stroke_node.h"
#include "trim_node.h"

namespace inae::model {
namespace AnimationNodeFactory {

std::unique_ptr<ShapeAnimationNode> node_for_shape_item(const ShapeItem *shape_item)
{
    std::unique_ptr<ShapeAnimationNode> result;
    switch (shape_item->type()) {
    case ShapeType::Group: {
        auto group = std::make_unique<GroupNode>(static_cast<const Group *>(shape_item));
        result = std::move(group);
    } break;
    case ShapeType::Shape: {
        auto shape = std::make_unique<ShapeNode>(static_cast<const Shape *>(shape_item));
        result = std::move(shape);
    } break;
        //    case ShapeType::Merge:
        //        break;
    case ShapeType::Transform: {
        auto shape_transform = std::make_unique<ShapeTransformationNode>(
            static_cast<const ShapeTransformation *>(shape_item));
        result = std::move(shape_transform);
    } break;
    case ShapeType::Fill: {
        auto fill = std::make_unique<FillNode>(static_cast<const Fill *>(shape_item));
        result = std::move(fill);
    } break;
    case ShapeType::Stroke: {
        auto stroke = std::make_unique<StrokeNode>(static_cast<const Stroke *>(shape_item));
        result = std::move(stroke);
    } break;
    case ShapeType::Ellipse: {
        auto ellipse = std::make_unique<EllipseNode>(static_cast<const Ellipse *>(shape_item));
        result = std::move(ellipse);
    } break;
    case ShapeType::Rectangle: {
        auto rect = std::make_unique<RectangleNode>(static_cast<const Rectangle *>(shape_item));
        result = std::move(rect);
    } break;
        //    case ShapeType::Star:
        //    {
        //        auto polystar = std::make_unique<PolyStarNode>(static_cast<const PolyStar &>(shape_item));
        //        result = std::move(polystar);
        //    }
        //    break;
    case ShapeType::Trim: {
        auto trim = std::make_unique<TrimNode>(static_cast<const Trim *>(shape_item));
        result = std::move(trim);
    } break;
    case ShapeType::Repeater: {
        auto repeater = std::make_unique<RepeaterNode>(static_cast<const Repeater *>(shape_item));
        result = std::move(repeater);
    } break;
    default: {
        const std::string &str = shape_type_to_name(shape_item->type());
        INAE_CORE_ERROR("Shape Item {} not Implemented!", str);
    } break;
    }

    return result;
}

} // namespace AnimationNodeFactory
} // namespace inae::model
