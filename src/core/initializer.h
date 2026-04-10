#ifndef INITIALIZER_H
#define INITIALIZER_H

#include "spdlog/common.h"
#include <string>

namespace alive::core {

class Initializer
{
public:
    struct Params
    {
        spdlog::level::level_enum log_level;
        std::string resource_path;
    };

    Initializer(const Params &param);
    ~Initializer();
};
} // namespace alive::core

#endif // INITIALIZER_H
