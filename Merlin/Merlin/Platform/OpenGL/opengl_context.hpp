#ifndef OPENGL_CONTEXT_HPP
#define OPENGL_CONTEXT_HPP
#include "Merlin/Render/graphics_context.hpp"


struct GLFWwindow;

namespace Merlin
{
    class OpenGLContext : public GraphicsContext
    {
        GLFWwindow* m_window;
    public:
        OpenGLContext(GLFWwindow* window);
        ~OpenGLContext();
        virtual void Init() override;
        virtual void SwapBuffers() override;
    };
}

#endif