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
        std::filesystem::path m_current_path = std::filesystem::current_path();

    public:
        inline std::string GetPanelName() { return m_name; }
        void DrawPanel();
    };
}  // namespace MerlinEditor
#endif