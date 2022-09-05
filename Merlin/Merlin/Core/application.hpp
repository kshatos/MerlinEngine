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
        std::string app_name = "";
        RenderBackend render_backend = RenderBackend::OpenGL;
        int window_width = 800;
        int window_height = 800;

        ApplicationInfo(std::string _app_name,
                        RenderBackend _render_backend,
                        int _window_width,
                        int _window_height)
            : app_name(_app_name)
            , render_backend(_render_backend)
            , window_width(_window_width)
            , window_height(_window_height)
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
        void Close();

    private:
        void HandleEvent(AppEvent& app_event);
        void UpdateLayers(float time_step);
    };
}  // namespace Merlin

#endif