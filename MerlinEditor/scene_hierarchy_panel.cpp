#include "scene_hierarchy_panel.hpp"

#include <imgui.h>

namespace MerlinEditor
{
    void SceneHierarchyPanel::DrawPanel()
    {
        ImGui::Begin(m_name.c_str());
        ImGui::End();
    }
}  // namespace MerlinEditor