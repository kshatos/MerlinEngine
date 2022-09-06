#ifndef INSPECTOR_PANEL_HPP
#define INSPECTOR_PANEL_HPP
#include <string>

namespace MerlinEditor
{
    class InspectorPanel
    {
        std::string m_name = "Inspector";

    public:
        inline std::string GetPanelName() { return m_name; }
        void DrawPanel();
    };
}  // namespace MerlinEditor

#endif