#include "editor_gui_layer.hpp"

#include <imgui_internal.h>  // Dockbuilder not exposed to public API yet, might churn

namespace MerlinEditor
{

    EditorGUILayer::EditorGUILayer()
        : m_active_scene(nullptr), first_frame(true)
    {
    }

    void EditorGUILayer::OnAttach()
    {
        m_active_scene = std::make_shared<Merlin::GameScene>();
        m_active_scene->CreateEntity();
        m_active_scene->CreateEntity();
        m_active_scene->CreateEntity();
        m_scene_hierarchy_panel.SetScene(m_active_scene);
    }

    void EditorGUILayer::OnDetatch() {}

    void EditorGUILayer::OnUpdate(float time_step)
    {
        DrawLayer();

        m_scene_hierarchy_panel.DrawPanel();
        m_asset_explorer_panel.DrawPanel();
        m_inspector_panel.DrawPanel();
        m_viewport_panel.DrawPanel();
    }

    void EditorGUILayer::HandleEvent(Merlin::AppEvent& app_event) {}

    void EditorGUILayer::DrawLayer()
    {
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        auto io = ImGui::GetIO();
        ImGui::SetNextWindowSize(io.DisplaySize);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGuiWindowFlags windowFlags =
            ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoDocking |
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar;
        ImGui::Begin("Main", NULL, windowFlags);
        ImGui::PopStyleVar(3);

        m_dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(
            m_dockspace_id, io.DisplaySize, ImGuiDockNodeFlags_None);

        if (first_frame)
        {
            ImGui::DockBuilderRemoveNode(m_dockspace_id);
            ImGui::DockBuilderAddNode(m_dockspace_id,
                                      ImGuiDockNodeFlags_DockSpace);
            ImGui::DockBuilderSetNodeSize(m_dockspace_id, io.DisplaySize);

            ImGuiID dock_main_id = m_dockspace_id;
            ImGuiID dock_id_right = ImGui::DockBuilderSplitNode(
                dock_main_id, ImGuiDir_Right, 0.30f, NULL, &dock_main_id);
            ImGuiID dock_id_bottom = ImGui::DockBuilderSplitNode(
                dock_main_id, ImGuiDir_Down, 0.30f, NULL, &dock_main_id);
            ImGuiID dock_id_left = ImGui::DockBuilderSplitNode(
                dock_main_id, ImGuiDir_Left, 0.40f, NULL, &dock_main_id);

            ImGui::DockBuilderDockWindow(
                m_scene_hierarchy_panel.GetPanelName().c_str(), dock_id_left);
            ImGui::DockBuilderDockWindow(
                m_asset_explorer_panel.GetPanelName().c_str(), dock_id_bottom);
            ImGui::DockBuilderDockWindow(
                m_inspector_panel.GetPanelName().c_str(), dock_id_right);
            ImGui::DockBuilderDockWindow(
                m_viewport_panel.GetPanelName().c_str(), dock_main_id);
            ImGui::DockBuilderFinish(m_dockspace_id);
            ImGui::DockBuilderDockWindow(
                m_viewport_panel.GetPanelName().c_str(), dock_main_id);

            ImGui::DockBuilderFinish(m_dockspace_id);
            first_frame = false;
        }

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Exit"))
                {
                    m_application->Close();
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Edit"))
            {
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
        ImGui::End();
    }

}  // namespace MerlinEditor