#ifndef GLFW_WINDOW_HPP
#define GLFW_WINDOW_HPP
#include <GLFW/glfw3.h>
#include "window.hpp"


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