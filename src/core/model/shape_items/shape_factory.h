#ifndef SHAPEFACTORY_H
#define SHAPEFACTORY_H

#include "core/model/shape_items/shape_item.h"
#include <core/alive_types/common_types.h>

namespace alive::model {

class ShapeFactory
{
public:
    static ShapeItem *create_shape_item(ShapeType type);
    // static std::vector<ShapeItem *> shapes_from_object(json::JsonObject &in_array,
    //                                                    SerializerWarnings &out_messages);
};

} // namespace alive::model

#endif // SHAPEFACTORY_H
