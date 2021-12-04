#include "Merlin/Core/input.hpp"
#include "Merlin/Core/glfw_input.hpp"


namespace Merlin
{
    std::unique_ptr<Input> Input::input_impl = std::make_unique<GLFWInput>();
}