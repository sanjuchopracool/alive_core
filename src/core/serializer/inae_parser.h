#ifndef INAE_PARSER_H
#define INAE_PARSER_H

#include <map>
#include <memory>
#include <string>
#include <variant>

namespace inae::model {
class Composition;
} // namespace inae::model

namespace inae::core::parser {

enum class ParserMessageType {
    e_Warning,
    e_Error,
};

struct ParserMessage
{
    ParserMessage(const std::string &msg)
        : message(msg)
    {}
    ParserMessage(ParserMessageType t, const std::string &msg)
        : type(t)
        , message(msg)
    {}

    ParserMessageType type = ParserMessageType::e_Warning;
    std::string message;
};

using ParserMessages = std::vector<ParserMessage>;
using ParserVariant = std::variant<int, float, std::string>;
using InaeProjectInformation = std::map<std::string, ParserVariant>;

std::unique_ptr<inae::model::Composition> parse_inae(const std::string &in_data,
                                                     InaeProjectInformation &out_info,
                                                     ParserMessages &out_messages);
} // namespace inae::core::parser

#endif // INAE_PARSER_H
