#include "Merlin/Core/glfw_input.hpp"
#include <glfw/glfw3.h>
#include "application.hpp"


namespace Merlin
{
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

}