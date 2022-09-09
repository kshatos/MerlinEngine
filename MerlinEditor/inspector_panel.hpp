#ifndef INSPECTOR_PANEL_HPP
#define INSPECTOR_PANEL_HPP
#include <Merlin/Scene/entity.hpp>
#include <optional>
#include <string>

#include "editor_panel.hpp"

namespace MerlinEditor
{
    class InspectorPanel : public EditorPanel
    {
        std::string m_name = "Inspector";

        std::optional<Merlin::Entity> m_inspected_entity;

    public:
        inline std::string GetPanelName() { return m_name; }
        void DrawPanel();
        inline void Inspect(std::optional<Merlin::Entity> entity)
        {
            m_inspected_entity = entity;
        }
    };
}  // namespace MerlinEditor

#endif