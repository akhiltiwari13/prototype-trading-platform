#include "DatabaseLayer/Logger/Logger.hpp"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace DatabaseLayer {

std::shared_ptr<spdlog::logger> Logger::s_logger;

void Logger::Initialize() {
    std::vector<spdlog::sink_ptr> logSinks;
    logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("database.log", true));

    logSinks[0]->set_pattern("%^[%T] %n: %v%$");
    logSinks[1]->set_pattern("[%T] [%l] %n: %v");

    s_logger = std::make_shared<spdlog::logger>("DATABASE", begin(logSinks), end(logSinks));
    spdlog::register_logger(s_logger);
    s_logger->set_level(spdlog::level::trace);
    s_logger->flush_on(spdlog::level::trace);
}

std::shared_ptr<spdlog::logger> Logger::GetLogger() {
    if (!s_logger) {
        Initialize();
    }
    return s_logger;
}

} // namespace DatabaseLayer