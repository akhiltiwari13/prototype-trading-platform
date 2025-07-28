#pragma once

#include <spdlog/spdlog.h>
#include <memory>

namespace DatabaseLayer {

/**
 * @brief Logging utilities for database operations
 */
class Logger {
public:
    static void Initialize();
    static std::shared_ptr<spdlog::logger> GetLogger();

private:
    static std::shared_ptr<spdlog::logger> s_logger;
};

// Convenience macros
#define DB_TRACE(...)    DatabaseLayer::Logger::GetLogger()->trace(__VA_ARGS__)
#define DB_DEBUG(...)    DatabaseLayer::Logger::GetLogger()->debug(__VA_ARGS__)
#define DB_INFO(...)     DatabaseLayer::Logger::GetLogger()->info(__VA_ARGS__)
#define DB_WARN(...)     DatabaseLayer::Logger::GetLogger()->warn(__VA_ARGS__)
#define DB_ERROR(...)    DatabaseLayer::Logger::GetLogger()->error(__VA_ARGS__)
#define DB_CRITICAL(...) DatabaseLayer::Logger::GetLogger()->critical(__VA_ARGS__)

} // namespace DatabaseLayer
