#include "Merlin/Scene/scene.hpp"
#include "Merlin/Render/renderer.hpp"


namespace Merlin
{
    void Scene::RenderScene()
    {
        Renderer::BeginScene(camera);
        for (const auto& entity : entities)
        {
            entity->OnBeforeRender();
            Renderer::Submit(
                entity->shader,
                entity->varray,
                entity->transform.GetTransformationMatrix());
        }
        Renderer::EndScene();
    }
    
    void Scene::OnUpdate(float time_step)
    {
        for (const auto& entity : entities)
        {
            entity->OnUpdate(time_step);
        }
    }

}