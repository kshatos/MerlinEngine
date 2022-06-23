#ifndef IMGUI_CONTEXT_HPP
#define IMGUI_CONTEXT_HPP
#include "Merlin/Render/render_api.hpp"
#include "Merlin/Core/app_events.hpp"
#include "imgui.h"


namespace Merlin
{
    class ImGuiAPI
    {
        ImGuiContext* context;
        RenderBackend backend;
        bool initialized = false;
    public:
        ~ImGuiAPI();
        void Init(void* window, std::shared_ptr<RenderAPI> renderApi);
        void HandleEvent(AppEvent& app_event);
    private:
        void InitImGuiImpl(void* window, std::shared_ptr<RenderAPI> renderApi);

    };
}
#endif