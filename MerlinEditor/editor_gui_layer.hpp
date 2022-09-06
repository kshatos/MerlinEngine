#ifndef EDITOR_GUI_LAYER_HPP
#define EDITOR_GUI_LAYER_HPP
#include <imgui.h>

#include <Merlin/Core/core.hpp>

#include "asset_explorer_panel.hpp"
#include "inspector_panel.hpp"
#include "scene_hierarchy_panel.hpp"
#include "viewport_panel.hpp"

namespace MerlinEditor
{
    class EditorGUILayer : public Merlin::Layer
    {
        ImGuiID m_dockspace_id;
        SceneHierarchyPanel m_scene_hierarchy_panel;
        AssetExplorerPanel m_asset_explorer_panel;
        InspectorPanel m_inspector_panel;
        ViewportPanel m_viewport_panel;
        bool first_frame = true;

        void OnAttach() override;

        void OnDetatch() override;

        void OnUpdate(float time_step) override;

        void HandleEvent(Merlin::AppEvent& app_event) override;

        void DrawLayer();
    };
}  // namespace MerlinEditor
#endif