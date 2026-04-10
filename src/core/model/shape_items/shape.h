#ifndef SHAPE_H
#define SHAPE_H

#include "shape_properties_macros.h"
#include <core/model/shape_items/shape_item.h>
#include <core/path_primitives/bezier_path.h>

namespace alive::core {
class Serializer;
}
namespace alive::model {

class Shape : public PathShapeItem
{
    ADD_PATH
public:
    Shape(Object *object = nullptr);
    ~Shape() override;

    Shape(Object *object, const Shape &other);
    Shape *clone_shape_item(Object *object) const override;

private:
    ADD_FRIEND_SERIALIZERS
};

} // namespace alive::model
#endif // SHAPE_H
