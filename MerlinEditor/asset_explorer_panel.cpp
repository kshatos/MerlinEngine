#include "asset_explorer_panel.hpp"

#include <imgui.h>

#include <Merlin/Core/logger.hpp>

namespace MerlinEditor
{
    void AssetExplorerPanel::DrawPanel()
    {
        ImGui::Begin(m_name.c_str());

        auto size = ImGui::GetContentRegionAvail();

        auto size1 = ImVec2(0.25f * size.x, size.y);
        auto size2 = ImVec2(size.x - size1.x, size.y);

        auto window_flags = ImGuiWindowFlags_AlwaysVerticalScrollbar;

        ImGui::BeginChild("Directories", size1, true, window_flags);
        DrawDirectoryTreeNode(m_project_path);
        ImGui::EndChild();

        ImGui::SameLine();
        ImGui::BeginChild("Assets", size2, true, window_flags);
        DrawDirectoryView();
        ImGui::EndChild();

        ImGui::End();
    }

    void AssetExplorerPanel::DrawDirectoryTreeNode(std::filesystem::path path)
    {
        bool has_subdirectory = false;
        std::filesystem::directory_iterator directory_iterator(path);
        for (const auto& entry : directory_iterator)
        {
            if (entry.is_directory())
            {
                has_subdirectory = true;
                break;
            }
        }

        ImGuiTreeNodeFlags tree_flags =
            ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnArrow;
        if (!has_subdirectory) tree_flags |= ImGuiTreeNodeFlags_Leaf;

        std::string path_string = path.stem().string();
        bool is_open = ImGui::TreeNodeEx(path_string.c_str(), tree_flags);

        if (is_open)
        {
            std::filesystem::directory_iterator directory_iterator(path);
            for (const auto& entry : directory_iterator)
            {
                if (entry.is_directory())
                {
                    DrawDirectoryTreeNode(entry.path());
                }
            }
            ImGui::TreePop();
        }
    }

    void AssetExplorerPanel::DrawDirectoryView()
    {
        bool back_clicked = ImGui::Button("BACK");
        ImGui::SameLine();

        auto relative_path = std::filesystem::relative(
            m_current_path, m_project_path.parent_path());
        auto relative_path_string = relative_path.string();
        ImGui::Text("%s", relative_path_string.c_str());

        ImGui::Separator();

        float cellSize = 200.0f;
        float panelWidth = ImGui::GetContentRegionAvail().x;
        int columnCount = (int)(panelWidth / cellSize);
        if (columnCount < 1) columnCount = 1;

        ImGui::Columns(columnCount, 0, false);

        std::filesystem::directory_iterator directory_iterator(m_current_path);
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

        if (back_clicked && m_current_path != m_project_path)
        {
            m_current_path = m_current_path.parent_path();
        }
    }

}  // namespace MerlinEditor