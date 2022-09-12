#ifndef ASSET_EXPLORER_PANEL_HPP
#define ASSET_EXPLORER_PANEL_HPP
#include <filesystem>
#include <string>

#include "editor_panel.hpp"

namespace MerlinEditor
{
    class AssetExplorerPanel : public EditorPanel
    {
        std::string m_name = "Asset Explorer";
        const std::string m_asset_path = "Assets";
        std::filesystem::path m_project_path = std::filesystem::current_path();
        std::filesystem::path m_current_path = std::filesystem::current_path();
        std::unordered_set<std::filesystem::path> m_registered_asset_paths;

    public:
        AssetExplorerPanel()
            : m_project_path(std::filesystem::current_path())
            , m_current_path(std::filesystem::current_path())
            , m_registered_asset_paths()
        {
        }
        inline std::string GetPanelName() { return m_name; }
        void DrawPanel();

    private:
        void DrawDirectoryTreeNode(std::filesystem::path path);
        void DrawDirectoryView();
        void DrawDirectoryEntry(std::filesystem::directory_entry);

        void ScanForAssets(std::filesystem::path directory_path);
    };
}  // namespace MerlinEditor
#endif