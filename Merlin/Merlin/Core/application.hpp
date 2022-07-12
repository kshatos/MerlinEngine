#ifndef APPLICATION_HPP
#define APPLICATION_HPP
#include "Merlin//Core/app_events.hpp"
#include "Merlin//Core/window.hpp"
#include "Merlin/Core/layer_stack.hpp"
#include "Merlin/Render/render_api.hpp"


namespace Merlin
{
    struct ApplicationInfo
    {
        std::string appName = "";
        RenderBackend renderBackend = RenderBackend::OPENGL;
        int windowWidth = 800;
        int windowHeight = 800;

        ApplicationInfo(
            std::string _appName,
            RenderBackend _renderBackend,
            int _windowWidth,
            int _windowHeight) :
            appName(_appName),
            renderBackend(_renderBackend),
            windowWidth(_windowWidth),
            windowHeight(_windowHeight)
        {
        }
    };


    class Application
    {
        static Application* app_instance;

        std::shared_ptr<Window> m_main_window;
        LayerStack m_layer_stack;
        bool m_is_running;
        float m_last_frame_time = 0.0f;
    public:
        inline static Application& Get() { return *app_instance; }
       
        Application(const ApplicationInfo& properties);
        virtual ~Application() {}

        void PushLayerFront(std::shared_ptr<Layer> layer);
        void PushLayerBack(std::shared_ptr<Layer> layer);

        inline std::shared_ptr<Window> GeMaintWindow() { return m_main_window; }
        void Run();
    private:
        void HandleEvent(AppEvent& app_event);
        void UpdateLayers(float time_step);
    };
}

#endif