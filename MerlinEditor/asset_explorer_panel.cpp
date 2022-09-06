#include "asset_explorer_panel.hpp"

#include <imgui.h>

namespace MerlinEditor
{
    void AssetExplorerPanel::DrawPanel()
    {
        ImGui::Begin(m_name.c_str());
        ImGui::End();
    }
}  // namespace MerlinEditor