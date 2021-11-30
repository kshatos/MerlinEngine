#include "glfw_window.hpp"
#include "app_events.hpp"


namespace Merlin
{
    bool GLFWWindowImpl::glfw_is_initialized = false;
    int GLFWWindowImpl::glfw_window_count = 0;

    GLFWWindowImpl::GLFWWindowImpl(WindowProperties props)
    {
        data.title = props.title;
        data.width = props.width;
        data.height = props.height;

        glfw_window_count++;
        if (!glfw_is_initialized)
        {
            glfwInit();
            glfw_is_initialized = true;
        }

        window_ptr = glfwCreateWindow(
            props.width, props.height, props.title.c_str(), NULL, NULL);

        glfwMakeContextCurrent(window_ptr);
        glfwSetWindowUserPointer(window_ptr, &data);
        glViewport(0, 0, props.width, props.height);

        SetGLFWCallbacks();
    }

    GLFWWindowImpl::~GLFWWindowImpl()
    {
        glfwDestroyWindow(window_ptr);

        glfw_window_count--;
        if (glfw_window_count <= 0 && glfw_is_initialized)
        {
            glfwTerminate();
            glfw_is_initialized = false;
        }
    }

    unsigned int GLFWWindowImpl::GetWidth()
    {
        return data.width;
    }

    unsigned int GLFWWindowImpl::GetHeight()
    {
        return data.height;
    }

    void GLFWWindowImpl::SetEventCallback(const EventCallbackFunction& callback)
    {
        data.callback = callback;
    }

    void GLFWWindowImpl::OnUpdate()
    {
        glfwPollEvents();
        glfwSwapBuffers(window_ptr);
    }

    void GLFWWindowImpl::SetGLFWCallbacks()
    {
        glfwSetWindowSizeCallback(window_ptr,
            [](GLFWwindow* window, int width, int height)
        {
            auto& x = *(GLFWWindowData*)glfwGetWindowUserPointer(window);
            x.width = width;
            x.height = height;
            WindowResizedEvent app_event(width, height);
            x.callback(app_event);
        });

        glfwSetWindowCloseCallback(window_ptr,
            [](GLFWwindow* window)
        {
            WindowClosedEvent app_event;
            auto& x = *(GLFWWindowData*)glfwGetWindowUserPointer(window);
            x.callback(app_event);
        });

        glfwSetKeyCallback(window_ptr,
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

        glfwSetMouseButtonCallback(window_ptr,
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

        glfwSetScrollCallback(window_ptr,
            [](GLFWwindow* window, double xoffset, double yoffset)
        {
            MouseScrolledEvent app_event((float)xoffset, (float)yoffset);
            auto& x = *(GLFWWindowData*)glfwGetWindowUserPointer(window);
            x.callback(app_event);
        });

        glfwSetCursorPosCallback(window_ptr,
            [](GLFWwindow* window, double xpos, double ypos)
        {
            MouseMovedEvent app_event((float)xpos, (float)ypos);
            auto& x = *(GLFWWindowData*)glfwGetWindowUserPointer(window);
            x.callback(app_event);
        });
    }

}