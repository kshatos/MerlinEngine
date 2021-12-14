#ifndef INPUT_HPP
#define INPUT_HPP
#include <memory>
#include "Merlin/Core/input_codes.hpp"
#include "Merlin/Core/app_events.hpp"
#include "glm/glm.hpp"


namespace Merlin
{
    struct InputState
    {
        glm::vec2 scroll_delta = glm::vec2(0.0f);
    };

    class Input
    {
        friend class Application;

        static std::unique_ptr<Input> input_impl;

        inline static void HandleEvent(AppEvent& app_event) { return input_impl->HandleEventImpl(app_event); }
        inline static void EndFrame() { input_impl->EndFrameImpl(); }
    public:
        inline static bool GetKeyDown(KeyCode key) { return input_impl->GetKeyDownImpl(key); }
        inline static bool GetMouseButtonDown(MouseCode button) { return input_impl->GetMouseButtonDownImpl(button); }
        inline static glm::vec2 GetMousePosition() { return input_impl->GetMousePositionImpl(); }
        inline static glm::vec2 GetMouseScrollDelta() { return input_impl->GetMouseScrollDeltaImpl(); }
    protected:
        virtual void HandleEventImpl(AppEvent& app_event) = 0;
        virtual void EndFrameImpl() = 0;
        virtual bool GetKeyDownImpl(KeyCode key) = 0;
        virtual bool GetMouseButtonDownImpl(MouseCode button) = 0;
        virtual glm::vec2 GetMousePositionImpl() = 0;
        virtual glm::vec2 GetMouseScrollDeltaImpl() = 0;
    };
}

#endif