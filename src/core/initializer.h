#ifndef INITIALIZER_H
#define INITIALIZER_H

#include <string_view>

namespace inae::core {
class Initializer
{
public:
    Initializer(std::string_view resource_path);
    ~Initializer();
};
} // namespace inae::core

#endif // INITIALIZER_H
