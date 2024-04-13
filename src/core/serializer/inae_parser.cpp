#include "inae_parser.h"
#include <core/inae_version.h>
#include <core/private/json.h>
#include <core/serializer/json_helper.h>

using namespace inae::core;
namespace {
const char k_inae_version[] = "inae_version";
} // namespace

std::unique_ptr<inae::model::Composition> inae::core::parser::parse_inae(
    const std::string &in_data, InaeProjectInformation &out_info, ParserMessages &out_messages)
{
    json::JsonObject obj = json::JsonObject::parse(in_data);
    if (obj.is_null())
        return nullptr;

    auto json = json::json_pop(obj, k_inae_version);
    inae::Version version;
    if (!json.is_null()) {
        std::string version_str = json;
        version = Version::from_string(version_str);
    }
    return nullptr;
}
