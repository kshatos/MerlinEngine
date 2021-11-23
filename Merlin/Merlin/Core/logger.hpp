#ifndef LOGGER_HPP
#define LOGGER_HPP
#include "spdlog/spdlog.h"


namespace Merlin
{
    class Logger
    {
        static std::shared_ptr<spdlog::logger> logger;

    public:
        static void Init();
        static std::shared_ptr<spdlog::logger>& GetLogger();
    };
}

#endif