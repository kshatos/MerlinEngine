#ifndef INPUT_HPP
#define INPUT_HPP
#include <memory>
#include "Merlin/Core/input_codes.hpp"
#include "glm/glm.hpp"


namespace Merlin
{
    class Input
    {
        static std::unique_ptr<Input> input_impl;

    public:
        inline static bool GetKeyDown(KeyCode key) { return input_impl->GetKeyDownImpl(key); }
        inline static bool GetMouseButtonDown(MouseCode button) { return input_impl->GetMouseButtonDownImpl(button); }
        inline static glm::vec2 GetMousePosition() { return input_impl->GetMousePositionImpl(); }
    protected:
        virtual bool GetKeyDownImpl(KeyCode key) = 0;
        virtual bool GetMouseButtonDownImpl(MouseCode button) = 0;
        virtual glm::vec2 GetMousePositionImpl() = 0;
    };
}

#endif