#include "initializer.h"
#include <core/logging/logger.h>
#include <core/resource_manager.h>

namespace inae::core {
static int init_counter = 0;

void init(const Initializer::Params &param)
{
    Logger::init(param.log_level);
    ResourceManager::init_from_path(param.resource_path);
}

void de_init()
{
    Logger::de_init();
}

Initializer::Initializer(const Params &param)
{
    if (init_counter++ == 0) {
        init(param);
    }
}

Initializer::~Initializer()
{
    if (--init_counter == 0) {
        de_init();
    }
}
} // namespace inae::core
