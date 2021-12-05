#ifndef IMGUI_LAYER_HPP
#define IMGUI_LAYER_HPP
#include "Merlin/Core/layer.hpp"


namespace Merlin
{
    class IMGUILayer : public Layer
    {
    public:
        void OnAttach() override;
        void OnDetatch() override;
        void OnUpdate() override;
        void HandleEvent(AppEvent& app_event) override;
    };
}

#endif
