#ifndef ASSET_EXPLORER_PANEL_HPP
#define ASSET_EXPLORER_PANEL_HPP
#include <string>

namespace MerlinEditor
{
    class AssetExplorerPanel
    {
        std::string m_name = "Asset Explorer";

    public:
        inline std::string GetPanelName() { return m_name; }
        void DrawPanel();
    };
}  // namespace MerlinEditor
#endif