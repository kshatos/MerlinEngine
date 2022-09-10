#ifndef ASSET_EXPLORER_PANEL_HPP
#define ASSET_EXPLORER_PANEL_HPP
#include <string>

#include "editor_panel.hpp"

namespace MerlinEditor
{
    class AssetExplorerPanel : public EditorPanel
    {
        std::string m_name = "Asset Explorer";

    public:
        inline std::string GetPanelName() { return m_name; }
        void DrawPanel();
    };
}  // namespace MerlinEditor
#endif