#include "Merlin/Platform/GLFW/glfw_window.hpp"
#include "Merlin/Core/app_events.hpp"
#ifdef MERLIN_USE_VULKAN
#include <vulkan/vulkan.h>
#endif
#ifdef MERLIN_USE_OPENGL
#include <glad/glad.h>
#endif
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>


namespace Merlin
{
    bool GLFWWindowImpl::glfw_is_initialized = false;
    int GLFWWindowImpl::glfw_window_count = 0;

    GLFWWindowImpl::GLFWWindowImpl(WindowProperties props)
    {
        m_data.title = props.title;
        m_data.width = props.width;
        m_data.height = props.height;

        glfw_window_count++;
        if (!glfw_is_initialized)
        {
            glfwInit();
            glfw_is_initialized = true;
        }

        if (props.renderBackend == RenderBackend::VULKAN)
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        m_window_ptr = glfwCreateWindow(
            props.width, props.height, props.title.c_str(), NULL, NULL);
        glfwSetWindowUserPointer(m_window_ptr, &m_data);
        SetGLFWCallbacks();

        m_renderApi = RenderAPI::Create(props.renderBackend);
        m_renderApi->Init(m_window_ptr);
    }

    GLFWWindowImpl::~GLFWWindowImpl()
    {
        m_renderApi->Shutdown();

        glfwDestroyWindow(m_window_ptr);

        glfw_window_count--;
        if (glfw_window_count <= 0 && glfw_is_initialized)
        {
            glfwTerminate();
            glfw_is_initialized = false;
        }
    }

    double GLFWWindowImpl::CurrentTime()
    {
        return glfwGetTime();
    }

    unsigned int GLFWWindowImpl::GetWidth()
    {
        return m_data.width;
    }

    unsigned int GLFWWindowImpl::GetHeight()
    {
        return m_data.height;
    }

    void GLFWWindowImpl::SetEventCallback(const EventCallbackFunction& callback)
    {
        m_data.callback = callback;
    }

    void GLFWWindowImpl::PollEvents()
    {
        glfwPollEvents();
    }

    void GLFWWindowImpl::BeginFrame()
    {
        m_renderApi->BeginFrame();
    }

    void GLFWWindowImpl::EndFrame()
    {
        m_renderApi->EndFrame();
    }

    void GLFWWindowImpl::PresentFrame()
    {
        m_renderApi->PresentFrame();
    }

    void GLFWWindowImpl::SetGLFWCallbacks()
    {
        glfwSetWindowSizeCallback(m_window_ptr,
            [](GLFWwindow* window, int width, int height)
            {
                auto& x = *(GLFWWindowData*)glfwGetWindowUserPointer(window);
                x.width = width;
                x.height = height;
                WindowResizedEvent app_event(width, height);
                x.callback(app_event);
            });

        glfwSetWindowCloseCallback(m_window_ptr,
            [](GLFWwindow* window)
            {
                WindowClosedEvent app_event;
                auto& x = *(GLFWWindowData*)glfwGetWindowUserPointer(window);
                x.callback(app_event);
            });

        glfwSetKeyCallback(m_window_ptr,
            [](GLFWwindow* window, int key, int scancode, int action, int mods)
            {
                auto& x = *(GLFWWindowData*)glfwGetWindowUserPointer(window);
                switch (action)
                {
                case GLFW_PRESS:
                {
                    KeyPressedEvent app_event(key, 0);
                    x.callback(app_event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent app_event(key);
                    x.callback(app_event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressedEvent app_event(key, 1);
                    x.callback(app_event);
                    break;
                }
                }
            });

        glfwSetMouseButtonCallback(m_window_ptr,
            [](GLFWwindow* window, int button, int action, int mods)
            {
                auto& x = *(GLFWWindowData*)glfwGetWindowUserPointer(window);
                switch (action)
                {
                case GLFW_PRESS:
                {
                    MouseButtonPressedEvent app_event(button);
                    x.callback(app_event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent app_event(button);
                    x.callback(app_event);
                    break;
                }
                }
            });

        glfwSetScrollCallback(m_window_ptr,
            [](GLFWwindow* window, double xoffset, double yoffset)
            {
                MouseScrolledEvent app_event((float)xoffset, (float)yoffset);
                auto& x = *(GLFWWindowData*)glfwGetWindowUserPointer(window);
                x.callback(app_event);
            });

        glfwSetCursorPosCallback(m_window_ptr,
            [](GLFWwindow* window, double xpos, double ypos)
            {
                MouseMovedEvent app_event((float)xpos, (float)ypos);
                auto& x = *(GLFWWindowData*)glfwGetWindowUserPointer(window);
                x.callback(app_event);
            });
    }

    void* GLFWWindowImpl::GetNativePointer()
    {
        return static_cast<void*>(m_window_ptr);
    }
}