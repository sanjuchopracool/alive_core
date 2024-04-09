#ifndef LOGGER_H
#define LOGGER_H

#include <memory>
#include <spdlog/spdlog.h>

namespace inae::Logger {

void init();
void de_init();

std::shared_ptr<spdlog::logger> &get_logger();

} // namespace inae::Logger

#define INAE_CORE_TRACE(...) Logger::get_logger()->trace(__VA_ARGS__)
#define INAE_CORE_DEBUG(...) Logger::get_logger()->debug(__VA_ARGS__)
#define INAE_CORE_INFO(...) Logger::get_logger()->info(__VA_ARGS__)
#define INAE_CORE_WARN(...) Logger::get_logger()->warn(__VA_ARGS__)
#define INAE_CORE_ERROR(...) Logger::get_logger()->error(__VA_ARGS__)
#define INAE_CORE_CRITICAL(...) Logger::get_logger()->critical(__VA_ARGS__)

#endif // LOGGER_H
