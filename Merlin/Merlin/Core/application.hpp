#ifndef APPLICATION_HPP
#define APPLICATION_HPP
#include "Merlin//Core/app_events.hpp"
#include "Merlin//Core/window.hpp"


namespace Merlin
{
    class Application
    {
        static Application* app_instance;

        std::shared_ptr<Window> main_window;
        bool is_running;
    public:
        inline static Application& Get() { return *app_instance; }
       
        Application();
        virtual ~Application() {}
        inline std::shared_ptr<Window> GeMaintWindow() { return main_window; }
        virtual void HandleEvent(AppEvent& app_event);
        virtual void OnUpdate() {}
        void Run();
    };
}

#endif