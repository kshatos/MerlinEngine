#include "opengl_context.hpp"
#include <GLFW/glfw3.h>


namespace Merlin
{
    OpenGLContext::OpenGLContext(GLFWwindow* window_handle)
    {
        window_ptr = window_handle;
    }

    OpenGLContext::~OpenGLContext()
    {
    }

    void OpenGLContext::Init()
    {
        glfwMakeContextCurrent(window_ptr);
    }

    void OpenGLContext::SwapBuffers()
    {
        glfwSwapBuffers(window_ptr);
    }
}
