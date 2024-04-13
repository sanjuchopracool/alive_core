#include "inae_api.h"
#include <core/initializer.h>
#include <filesystem>
#include <string>

using namespace inae::core;
namespace {
std::unique_ptr<Initializer> core_initializer;
}

uint8_t inae_init(char *error)
{
    uint8_t status = 1;
    if (!core_initializer) {
        try {
            core_initializer = std::make_unique<Initializer>(
                std::string_view("/Users/sanju/PROJECTS/inae_resources"));
            status = 0;
        } catch (const std::filesystem::filesystem_error &e) {
            const std::string str(e.what());
            strncpy(error, str.c_str(), str.length());
            error[str.length()] = '\0';
            status = 1;
        }
    } else {
        status = 1;
    }
    return status;
}

uint8_t inae_de_init(char *error)
{
    if (core_initializer) {
        core_initializer.reset();
        return 0;
    } else {
        return 1;
    }
}
