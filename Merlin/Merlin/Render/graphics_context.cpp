#include "graphics_context.hpp"
#include "opengl_context.hpp"


namespace Merlin
{
    // TODO: Make platform specific
    GraphicsContext* GraphicsContext::Create(void* window)
    {
        return new OpenGLContext((GLFWwindow*)window);
    }
}