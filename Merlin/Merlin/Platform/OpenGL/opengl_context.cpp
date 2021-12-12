#include "opengl_context.hpp"
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>


namespace Merlin
{
    OpenGLContext::OpenGLContext(GLFWwindow* window)
    {
        m_window = window;
    }

    OpenGLContext::~OpenGLContext()
    {
    }

    void OpenGLContext::Init()
    {
        glfwMakeContextCurrent(m_window);
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    }

    void OpenGLContext::SwapBuffers()
    {
        glfwSwapBuffers(m_window);
    }
}
