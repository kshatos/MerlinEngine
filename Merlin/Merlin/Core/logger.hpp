#ifndef LOGGER_HPP
#define LOGGER_HPP
#include<string>


namespace Merlin
{
    class Logger
    {
    public:
        static void Init();
        static void Info(const std::string& message);
        static void Warn(const std::string& message);
        static void Error(const std::string& message);
        static void Fatal(const std::string& message);
    };

#ifdef MERLIN_USE_LOGGING
#define ME_LOG_INFO(x) Merlin::Logger::Info(x)
#define ME_LOG_WARN(x) Merlin::Logger::Warn(x)
#define ME_LOG_ERROR(x) Merlin::Logger::Error(x)
#define ME_LOG_FATAL(x) Merlin::Logger::Fatal(x)
#else
#define ME_LOG_INFO(x) 
#define ME_LOG_WARN(x)
#define ME_LOG_ERROR(x)
#define ME_LOG_FATAL(x)
#endif
}
#endif