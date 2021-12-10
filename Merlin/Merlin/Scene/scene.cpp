#include "Merlin/Scene/scene.hpp"
#include "Merlin/Render/renderer.hpp"
#include "Merlin/Render/shader.hpp"
#include "Merlin/Render/vertex_array.hpp"
#include "Merlin/Render/transform.hpp"
#include "Merlin/Scene/core_components.hpp"


namespace Merlin
{
    void Scene::RenderScene()
    {
        Renderer::BeginScene(camera);
        for (const auto& entity : entities)
        {
            auto point_light_comp = entity->GetComponent<PointLightComponent>();
            if (point_light_comp != nullptr)
            {
                Renderer::AddPointLight(point_light_comp->data);
            }
        }
        for (const auto& entity : entities)
        {
            auto mesh_comp = entity->GetComponent<MeshRenderComponent>();
            auto transform_comp = entity->GetComponent<TransformComponent>();
            if (mesh_comp != nullptr && transform_comp != nullptr)
            {
                Renderer::Submit(
                    mesh_comp->shader,
                    mesh_comp->varray,
                    transform_comp->transform.GetTransformationMatrix());
            }
        }
        Renderer::EndScene();
    }
    
    void Scene::OnAwake()
    {
        for (const auto& entity : entities)
        {
            entity->OnAwake();
        }
    }

    void Scene::OnUpdate(float time_step)
    {
        for (const auto& entity : entities)
        {
            entity->OnUpdate(time_step);
        }
    }

}