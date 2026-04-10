#ifndef JSON_H
#define JSON_H

#include <nlohmann/json.hpp>

namespace alive::json {
using JsonObject = nlohmann::json;
using OrderedJsonObject = nlohmann::ordered_json;
} // namespace alive::json
#endif // JSON_H
