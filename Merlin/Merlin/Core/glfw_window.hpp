#ifndef GLFW_WINDOW_HPP
#define GLFW_WINDOW_HPP
#include "window.hpp"
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

        GLFWwindow* window_ptr;
        GLFWWindowData data;
        std::unique_ptr<GraphicsContext> context;

        void SetGLFWCallbacks();
    public:
        GLFWWindowImpl(WindowProperties props);
        ~GLFWWindowImpl();
        unsigned int GetWidth() override;
        unsigned int GetHeight() override;
        void SetEventCallback(const EventCallbackFunction& callback) override;
        void OnUpdate() override;
    };
}
#endif