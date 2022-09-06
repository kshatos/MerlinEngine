#ifndef SCENE_HIERARCHY_PANEL_HPP
#define SCENE_HIERARCHY_PANEL_HPP
#include <Merlin/Scene/scene.hpp>
#include <optional>

namespace MerlinEditor
{
    class SceneHierarchyPanel
    {
        std::string m_name = "Scene Hierarchy";
        std::shared_ptr<Merlin::GameScene> m_scene = nullptr;
        std::optional<Merlin::Entity> m_selected_entity;

    public:
        SceneHierarchyPanel();
        inline std::string GetPanelName() { return m_name; }
        inline void SetScene(std::shared_ptr<Merlin::GameScene> scene)
        {
            m_scene = scene;
        }
        void DrawPanel();

    private:
        void DrawEntity(Merlin::Entity entity);
    };
}  // namespace MerlinEditor
#endif