#ifndef APPLICATION_HPP
#define APPLICATION_HPP
#include "Merlin//Core/app_events.hpp"
#include "Merlin//Core/window.hpp"
#include "Merlin/Core/layer_stack.hpp"


namespace Merlin
{
    class Application
    {
        static Application* app_instance;

        LayerStack layer_stack;
        std::shared_ptr<Window> main_window;
        bool is_running;
        float last_frame_time = 0.0f;
    public:
        inline static Application& Get() { return *app_instance; }
       
        Application();
        virtual ~Application() {}

        void PushLayerFront(std::shared_ptr<Layer> layer);
        void PushLayerBack(std::shared_ptr<Layer> layer);

        inline std::shared_ptr<Window> GeMaintWindow() { return main_window; }
        void Run();
    private:
        void HandleEvent(AppEvent& app_event);
        void OnUpdate(float time_step);
    };
}

#endif