#ifndef SHAPESERIALIZER_H
#define SHAPESERIALIZER_H

#include <core/common_types.h>
#include <core/model/shape_items/shape_item.h>
#include <core/private/json.h>
namespace inae::model {
class ShapeItem;
}

namespace inae::core {
namespace ShapeSerializer {

model::ShapeItem *shape_from_object(json::JsonObject &in_object,
                                    SerializerWarnings &out_messages,
                                    const Version &version);
std::string shape_type_to_string(model::ShapeType type);
void to_json_object(const model::ShapeItems &shape_items, json::OrderedJsonObject &shapes_json);

} // namespace ShapeSerializer
} // namespace inae::core

#endif // SHAPESERIALIZER_H
