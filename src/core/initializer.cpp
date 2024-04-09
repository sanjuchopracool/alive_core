#include "initializer.h"
#include <core/logging/logger.h>
#include <core/resource_manager.h>

namespace inae::core {
static int init_counter = 0;

void init(std::string_view resource_path)
{
    Logger::init();
    ResourceManager::init_from_path(resource_path);
}

void de_init()
{
    Logger::de_init();
}

Initializer::Initializer(std::string_view resource_path)
{
    if (init_counter++ == 0) {
        init(resource_path);
    }
}

Initializer::~Initializer()
{
    if (--init_counter == 0) {
        de_init();
    }
}
} // namespace inae::core
