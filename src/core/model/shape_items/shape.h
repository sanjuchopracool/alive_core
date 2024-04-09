#ifndef SHAPE_H
#define SHAPE_H

#include "shape_properties_macros.h"
#include <core/model/shape_items/shape_item.h>
#include <core/path_primitives/bezier_path.h>

namespace inae::core {
class Serializer;
}
namespace inae::model {

class Shape : public PathShapeItem
{
    ADD_PATH
public:
    Shape(Object *object = nullptr);
    ~Shape() override;

private:
    friend class inae::core::Serializer;
};

} // namespace inae::model
#endif // SHAPE_H
