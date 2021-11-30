#include "window.hpp"
#include "glfw_window.hpp"


namespace Merlin
{
    Window* Window::Create(WindowProperties props)
    {
        // TODO: Add platform dependant code here, leave GLFW for now as its the only implementation
        return new GLFWWindowImpl(props);
    }
}