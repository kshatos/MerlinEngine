#include "inspector_panel.hpp"

#include <imgui.h>

namespace MerlinEditor
{
    void InspectorPanel::DrawPanel()
    {
        ImGui::Begin(m_name.c_str());
        ImGui::End();
    }
}  // namespace MerlinEditor