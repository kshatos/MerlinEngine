#ifndef OPENGL_CONTEXT_HPP
#define OPENGL_CONTEXT_HPP
#include "graphics_context.hpp"


struct GLFWwindow;

namespace Merlin
{
    class OpenGLContext : public GraphicsContext
    {
        GLFWwindow* window_ptr;
    public:
        OpenGLContext(GLFWwindow* window_handle);
        ~OpenGLContext();
        virtual void Init() override;
        virtual void SwapBuffers() override;
    };
}

#endif