#include "Merlin/Core/logger.hpp"
#include "Merlin/Core/window.hpp"
using namespace Merlin;

bool is_running = true;

void EventCallback(AppEvent& app_event)
{
    ME_LOG_INFO(app_event.ToString());

    AppEvent::Dispatch<WindowClosedEvent>(app_event,
        [](WindowClosedEvent& e)
    {
        is_running = false;
        return true;
    });
}


void main()
{
    Logger::Init();

    auto window = std::unique_ptr<Window>(Window::Create(WindowProperties("asdf", 600, 800)));
    window->SetEventCallback(EventCallback);
    while (is_running)
    {
        window->OnUpdate();
    }
}