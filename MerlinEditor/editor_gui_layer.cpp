#include "editor_gui_layer.hpp"

namespace MerlinEditor
{
    void EditorGUILayer::OnAttach() {}

    void EditorGUILayer::OnDetatch() {}

    void EditorGUILayer::OnUpdate(float time_step)
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
        ImGui::DockSpace(
            dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

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

    void EditorGUILayer::HandleEvent(Merlin::AppEvent& app_event) {}

}  // namespace MerlinEditor