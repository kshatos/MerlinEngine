#include "Merlin/Core/logger.hpp"

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

namespace Merlin
{
    std::shared_ptr<spdlog::logger> logger;

    void Logger::Init()
    {
        spdlog::set_pattern("[%T][%t]%^[%L] %v%$");

        logger = spdlog::stdout_color_mt("Merlin");
        logger->set_level(spdlog::level::trace);
    }

    void Logger::Info(const std::string& message) { logger->info(message); }

    void Logger::Warn(const std::string& message) { logger->warn(message); }

    void Logger::Error(const std::string& message) { logger->error(message); }

    void Logger::Fatal(const std::string& message)
    {
        logger->critical(message);
    }
}  // namespace Merlin
