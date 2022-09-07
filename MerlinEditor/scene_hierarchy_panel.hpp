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
        std::optional<Merlin::Entity> m_draged_entity;
        std::vector<Merlin::Entity> m_deleted_entities;
        

    public:
        SceneHierarchyPanel();
        inline std::string GetPanelName() { return m_name; }
        inline void SetScene(std::shared_ptr<Merlin::GameScene> scene)
        {
            m_scene = scene;
        }
        void DrawPanel();
        inline std::optional<Merlin::Entity> GetSelectedEntity()
        {
            return m_selected_entity;
        }

    private:
        void DrawEntity(Merlin::Entity entity, bool first_layer);
    };
}  // namespace MerlinEditor
#endif