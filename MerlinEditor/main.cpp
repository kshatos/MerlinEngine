#include <imgui.h>

#include "Merlin/Core/core.hpp"

using namespace Merlin;

class EditorGUILayer : public Layer
{
    void OnAttach() override {}

    void OnDetatch() override {}

    void OnUpdate(float time_step) override { ImGui::ShowDemoWindow(); }

    void HandleEvent(AppEvent& app_event) override {}
};

class MerlinEditorApp : public Application
{
public:
    MerlinEditorApp(const ApplicationInfo& properties) : Application(properties)
    {
        PushLayerBack(std::make_shared<EditorGUILayer>());
    }
};

void main()
{
    ApplicationInfo app_info{
        "Merlin Editor",
        RenderBackend::Vulkan,
        800,
        800,
    };
    MerlinEditorApp app(app_info);
    app.Run();
}