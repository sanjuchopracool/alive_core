#ifndef JSON_H
#define JSON_H

#include <nlohmann/json.hpp>

namespace inae::json {
using JsonObject = nlohmann::json;
using OrderedJsonObject = nlohmann::ordered_json;
} // namespace inae::json
#endif // JSON_H
