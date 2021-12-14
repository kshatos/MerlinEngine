#ifndef GLFW_INPUT_HPP
#define GLFW_INPUT_HPP
#include "Merlin/Core/input.hpp"


namespace Merlin
{
    class GLFWInput : public Input
    {
        InputState input_state;
    protected:
        void HandleEventImpl(AppEvent& app_event) override;
        void EndFrameImpl() override;
        bool GetKeyDownImpl(KeyCode key) override;
        bool GetMouseButtonDownImpl(MouseCode button) override;
        glm::vec2 GetMousePositionImpl() override;
        glm::vec2 GetMouseScrollDeltaImpl() override;
    };
}
#endif