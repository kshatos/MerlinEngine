#include "Merlin/Core/logger.hpp"


void main()
{
    Merlin::Logger::Init();
    Merlin::Logger::GetLogger()->info("Hello World");
    Merlin::Logger::GetLogger()->warn("Uh-Oh");
    Merlin::Logger::GetLogger()->error("Yikes");
}