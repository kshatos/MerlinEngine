#include "window.hpp"

#include "Merlin/Platform/GLFW/glfw_window.hpp"

namespace Merlin
{
    Window* Window::Create(WindowProperties props)
    {
        // TODO: Add platform dependant code here, leave GLFW for now as its the
        // only implementation
        return new GLFWWindowImpl(props);
    }
}  // namespace Merlin