#ifndef INITIALIZER_H
#define INITIALIZER_H

#include "spdlog/common.h"
#include <string>

namespace inae::core {

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
} // namespace inae::core

#endif // INITIALIZER_H
