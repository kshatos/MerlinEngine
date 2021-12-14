#include "Merlin/Platform/GLFW/glfw_input.hpp"
#include <glfw/glfw3.h>
#include "Merlin/Core/application.hpp"


namespace Merlin
{
    void GLFWInput::HandleEventImpl(AppEvent& app_event)
    {
        app_event.Dispatch<MouseScrolledEvent>(
            [this](const MouseScrolledEvent& e)
        {
            input_state.scroll_delta.x += e.GetXScrollDelta();
            input_state.scroll_delta.y += e.GetYScrollDelta();
            return false;
        });
    }

    void GLFWInput::EndFrameImpl()
    {
        input_state.scroll_delta *= 0.0f;
    }

    bool GLFWInput::GetKeyDownImpl(KeyCode key)
    {
        GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GeMaintWindow()->GetNativePointer());
        auto state = glfwGetKey(window, static_cast<int32_t>(key));
        return (state == GLFW_PRESS);
    }

    bool GLFWInput::GetMouseButtonDownImpl(MouseCode button)
    {
        GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GeMaintWindow()->GetNativePointer());
        auto state = glfwGetMouseButton(window, button);
        return state == GLFW_PRESS;
    }

    glm::vec2 GLFWInput::GetMousePositionImpl()
    {
        GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GeMaintWindow()->GetNativePointer());
        double x, y = 0.0;
        glfwGetCursorPos(window, &x, &y);

        return glm::vec2(x, y);
    }

    glm::vec2 GLFWInput::GetMouseScrollDeltaImpl()
    {
        return input_state.scroll_delta;
    }

}