#ifndef SCENE_HIERARCHY_PANEL_HPP
#define SCENE_HIERARCHY_PANEL_HPP
#include <Merlin/Scene/scene.hpp>

namespace MerlinEditor
{
    class SceneHierarchyPanel
    {
        std::string m_name = "Scene Hierarchy";
        std::shared_ptr<Merlin::GameScene> m_scene;

    public:
        inline std::string GetPanelName() { return m_name; }
        inline void SetScene(std::shared_ptr<Merlin::GameScene> scene)
        {
            m_scene = scene;
        }
        void DrawPanel();
    };
}  // namespace MerlinEditor
#endif