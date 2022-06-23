#ifndef IMGUI_CONTEXT_HPP
#define IMGUI_CONTEXT_HPP
#include "Merlin/Render/render_api.hpp"
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
        void Init(std::shared_ptr<RenderAPI> renderApi);
    private:
        void InitImGuiImpl(std::shared_ptr<RenderAPI> renderApi);

    };
}
#endif