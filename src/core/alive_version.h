#ifndef ALIVE_VERSION_H
#define ALIVE_VERSION_H

#include <cstdint>
#include <string>
#include <vector>

namespace alive {
class Version
{
public:
    Version(uint8_t major = 0, uint8_t minor = 0, uint8_t micro = 1)
        : m_major(major)
        , m_minor(minor)
        , m_micro(micro)
    {}

    bool operator==(const Version &other) const = default;
    auto operator<=>(const Version &oth) const = default;

    std::string to_string() const
    {
        return std::to_string(m_major) + '.' + std::to_string(m_minor) + '.'
               + std::to_string(m_micro);
    }

    static Version from_string(std::string &s)
    {
        std::string sep = ".";
        size_t pos = 0;
        std::string token;
        std::vector<uint8_t> tokens;
        while ((pos = s.find(sep)) != std::string::npos) {
            token = s.substr(0, pos);
            uint8_t val = std::stoi(token);
            tokens.emplace_back(val);
            s.erase(0, pos + sep.length());
        }
        if (s.length()) {
            uint8_t val = std::stoi(s);
            tokens.emplace_back(val);
        }

        if (tokens.size() == 3) {
            return Version(tokens[0], tokens[1], tokens[2]);
        } else {
            return Version{};
        }
    }

private:
    uint8_t m_major;
    uint8_t m_minor;
    uint8_t m_micro;
};
} // namespace alive
#endif // ALIVE_VERSION_H
