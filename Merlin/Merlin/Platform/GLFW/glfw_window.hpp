#ifndef GLFW_WINDOW_HPP
#define GLFW_WINDOW_HPP
#include "Merlin/Core/window.hpp"
#include "Merlin/Render/graphics_context.hpp"


struct GLFWwindow;

namespace Merlin
{
    struct GLFWWindowData
    {
        std::string title;
        int width;
        int height;
        Window::EventCallbackFunction callback;
    };

    class GLFWWindowImpl : public Window
    {
        static bool glfw_is_initialized;
        static int glfw_window_count;

        GLFWwindow* m_window_ptr;
        GLFWWindowData m_data;
        std::unique_ptr<GraphicsContext> m_context;

        void SetGLFWCallbacks();
    public:
        GLFWWindowImpl(WindowProperties props);
        ~GLFWWindowImpl();
        double CurrentTime() override;
        unsigned int GetWidth() override;
        unsigned int GetHeight() override;
        void SetEventCallback(const EventCallbackFunction& callback) override;
        void OnUpdate() override;
        void* GetNativePointer() override;
        void HandleEvent(AppEvent& app_event) override;
    };
}
#endif