#ifndef VIEWPORT_PANEL_HPP
#define VIEWPORT_PANEL_HPP
#include <string>

namespace MerlinEditor
{
    class ViewportPanel
    {
        std::string m_name = "Viewport";

    public:
        inline std::string GetPanelName() { return m_name; }
        void DrawPanel();
    };
}  // namespace MerlinEditor
#endif