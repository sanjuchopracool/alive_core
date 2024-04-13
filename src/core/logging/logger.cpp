#include "logger.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace inae::Logger {

std::shared_ptr<spdlog::logger> main_logger;

void init()
{
    std::vector<spdlog::sink_ptr> log_sinks;
    log_sinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    log_sinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("inae.log", true));

    log_sinks[0]->set_pattern("[%l] %n: %v");
    log_sinks[1]->set_pattern("[%T] [%l] %n: %v");

    main_logger = std::make_shared<spdlog::logger>("main", begin(log_sinks), end(log_sinks));
    spdlog::register_logger(main_logger);
    main_logger->set_level(spdlog::level::trace);
    main_logger->flush_on(spdlog::level::trace);
}
void de_init()
{
    main_logger.reset();
}

std::shared_ptr<spdlog::logger> &get_logger()
{
    return main_logger;
}

void set_core_logger_level(spdlog::level::level_enum level)
{
    if (main_logger) {
        main_logger->set_level(level);
    }
}

} // namespace inae::Logger
