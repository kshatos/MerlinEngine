#include <imgui.h>

#include "Merlin/Core/core.hpp"

using namespace Merlin;

class EditorGUILayer : public Layer
{
    void OnAttach() override {}

    void OnDetatch() override {}

    void OnUpdate(float time_step) override
    {
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        auto io = ImGui::GetIO();
        ImGui::SetNextWindowContentSize(io.DisplaySize);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGuiWindowFlags windowFlags =
            ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoDocking |
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoBackground;

        ImGui::Begin("Main", NULL, windowFlags);
        ImGui::PopStyleVar(3);

        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Exit"))
                {
                    m_application->Close();
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        ImGui::End();
        ImGui::ShowDemoWindow();
    }

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
        RenderBackend::OpenGL,
        800,
        800,
    };
    MerlinEditorApp app(app_info);
    app.Run();
}