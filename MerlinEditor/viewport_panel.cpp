#include "viewport_panel.hpp"

#include <imgui.h>

namespace MerlinEditor
{
    void ViewportPanel::DrawPanel()
    {
        ImGui::Begin(m_name.c_str());
        ImGui::End();
    }
}  // namespace MerlinEditor