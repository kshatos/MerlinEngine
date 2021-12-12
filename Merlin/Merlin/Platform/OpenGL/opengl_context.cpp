#include "opengl_context.hpp"
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
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
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    }

    void OpenGLContext::SwapBuffers()
    {
        glfwSwapBuffers(window_ptr);
    }
}
