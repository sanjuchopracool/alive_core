#ifndef SHAPEFACTORY_H
#define SHAPEFACTORY_H

#include <core/common_types.h>
#include <core/private/json.h>

namespace inae::model {
class ShapeItem;

class ShapeFactory
{
public:
    static std::vector<ShapeItem *> shapes_from_object(json::JsonObject &in_array,
                                                       SerializerWarnings &out_messages);
};

} // namespace inae::model

#endif // SHAPEFACTORY_H
