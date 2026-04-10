#ifndef SHAPESERIALIZER_H
#define SHAPESERIALIZER_H

#include <core/alive_types/common_types.h>
#include <core/model/shape_items/shape_item.h>
#include <core/private/json.h>
namespace alive::model {
class ShapeItem;
}

namespace alive::core {

class SerializationContext;
namespace ShapeSerializer {

model::ShapeItem *shape_from_object(json::JsonObject &in_object,
                                    core::SerializationContext &out_messages,
                                    const Version &version);
std::string shape_type_to_string(model::ShapeType type);
void to_json_object(const model::ShapeItems &shape_items, json::OrderedJsonObject &shapes_json);

} // namespace ShapeSerializer
} // namespace alive::core

#endif // SHAPESERIALIZER_H
