#include "asset_explorer_panel.hpp"

#include <imgui.h>

namespace MerlinEditor
{
    void AssetExplorerPanel::DrawPanel()
    {
        ImGui::Begin(m_name.c_str());

        bool go_back = ImGui::Button("BACK");
        ImGui::SameLine();
        auto current_path_string = m_current_path.string();
        ImGui::Text("%s", current_path_string.c_str());

        ImGui::Separator();

        float cellSize = 200.0f;
        float panelWidth = ImGui::GetContentRegionAvail().x;
        int columnCount = (int)(panelWidth / cellSize);
        if (columnCount < 1) columnCount = 1;

        ImGui::Columns(columnCount, 0, false);

        std::filesystem::directory_iterator directory_iterator(
            m_current_path);
        for (const auto& entry : directory_iterator)
        {
            const auto& path = entry.path();
            std::string filename_string = path.filename().string();
            if (ImGui::Button(filename_string.c_str()))
            {
                if (entry.is_directory())
                {
                    m_current_path = entry.path();
                }
            }

            ImGui::NextColumn();
        }

        if (go_back)
        {
            m_current_path = m_current_path.parent_path();
        }
        ImGui::End();
    }
}  // namespace MerlinEditor