#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <vector>

namespace alive {

enum class MessageType {
    e_Warning,
    e_Error,
};

struct Message
{
    Message(const std::string &msg)
        : message(msg)
    {}
    Message(MessageType t, const std::string &msg)
        : type(t)
        , message(msg)
    {}

    MessageType type = MessageType::e_Warning;
    std::string message;
};

using Messages = std::vector<Message>;
} // namespace alive
#endif // MESSAGE_H
