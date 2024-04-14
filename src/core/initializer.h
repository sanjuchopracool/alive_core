#ifndef INITIALIZER_H
#define INITIALIZER_H

#include "spdlog/common.h"
#include <string_view>

namespace inae::core {

class Initializer
{
public:
    struct Params
    {
        spdlog::level::level_enum log_level;
        std::string_view resource_path;
    };

    Initializer(const Params &param);
    ~Initializer();
};
} // namespace inae::core

#endif // INITIALIZER_H
