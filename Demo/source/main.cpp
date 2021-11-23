#include "Merlin/Core/logger.hpp"


void main()
{
    Merlin::Logger::Init();
    ME_LOG_INFO("Hello World");
    ME_LOG_WARN("Oh-oh");
    ME_LOG_ERROR("Yikes");
    ME_LOG_FATAL("Oof");
}