#include "Merlin/Core/logger.hpp"
#include "spdlog/sinks/stdout_color_sinks.h"


namespace Merlin
{
    std::shared_ptr<spdlog::logger> Logger::logger;
    
    void Logger::Init()
    {
        spdlog::set_pattern("[%T][%t]%^[%L] %v%$");

        logger = spdlog::stdout_color_mt("Merlin");
        logger->set_level(spdlog::level::trace);
    }

    std::shared_ptr<spdlog::logger>& Logger::GetLogger()
    {
        return logger;
    }
}
