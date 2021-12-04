#include "Merlin/Core/glfw_input.hpp"
#include <glfw/glfw3.h>


namespace Merlin
{
    bool GLFWInput::GetKeyDownImpl(KeyCode key)
    {
        GLFWwindow* window = nullptr;
        auto state = glfwGetKey(window, key);
        return (
            key == GLFW_PRESS ||
            key == GLFW_REPEAT);
    }

    bool GLFWInput::GetMouseButtonDownImpl(MouseCode button)
    {
        GLFWwindow* window = nullptr;
        auto state = glfwGetMouseButton(window, button);
        return state == GLFW_PRESS;
    }

    glm::vec2 GLFWInput::GetMousePositionImpl()
    {
        GLFWwindow* window = nullptr;
        double x, y = 0.0;
        glfwGetCursorPos(window, &x, &y);

        return glm::vec2(x, y);
    }

}