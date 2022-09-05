#ifndef EDITOR_GUI_LAYER_HPP
#define EDITOR_GUI_LAYER_HPP
#include <imgui.h>

#include "Merlin/Core/core.hpp"

namespace MerlinEditor
{
    class EditorGUILayer : public Merlin::Layer
    {
        void OnAttach() override;

        void OnDetatch() override;

        void OnUpdate(float time_step) override;

        void HandleEvent(Merlin::AppEvent& app_event) override;
    };
}  // namespace MerlinEditor
#endif