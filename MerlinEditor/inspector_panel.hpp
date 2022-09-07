#ifndef INSPECTOR_PANEL_HPP
#define INSPECTOR_PANEL_HPP
#include <Merlin/Scene/entity.hpp>
#include <optional>
#include <string>

namespace MerlinEditor
{
    class InspectorPanel
    {
        std::string m_name = "Inspector";

        std::optional<Merlin::Entity> m_inspected_entity;

    public:
        inline std::string GetPanelName() { return m_name; }
        void DrawPanel();
        inline void Inspect(Merlin::Entity entity)
        {
            m_inspected_entity = entity;
        }
    };
}  // namespace MerlinEditor

#endif