#include "Merlin/Render/graphics_context.hpp"
#include "Merlin/Platform/OpenGL/opengl_context.hpp"


namespace Merlin
{
    // TODO: Make platform specific
    GraphicsContext* GraphicsContext::Create(void* window)
    {
        return new OpenGLContext((GLFWwindow*)window);
    }
}