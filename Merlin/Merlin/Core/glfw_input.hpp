#ifndef GLFW_INPUT_HPP
#define GLFW_INPUT_HPP
#include "Merlin/Core/input.hpp"


namespace Merlin
{
    class GLFWInput : public Input
    {
    protected:
        bool GetKeyDownImpl(KeyCode key) override;
        bool GetMouseButtonDownImpl(MouseCode button) override;
        glm::vec2 GetMousePositionImpl() override;
    };
}
#endif