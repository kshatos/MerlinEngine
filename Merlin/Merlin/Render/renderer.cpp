#include "Merlin/Render/renderer.hpp"
#include "Merlin/Render/render_api.hpp"
#include "Merlin/Render/shader.hpp"


namespace Merlin
{
    std::unique_ptr<RenderAPI> Renderer::render_impl = nullptr;
    std::unique_ptr<Renderer::SceneData> Renderer::scene_data = nullptr;

    void Renderer::Init()
    {
        render_impl = std::unique_ptr<RenderAPI>(RenderAPI::Create());
        render_impl->Init();
        scene_data = std::make_unique<Renderer::SceneData>();
    }

    void Renderer::BeginScene(
        const std::shared_ptr<Camera>& camera)
    {
        scene_data->view_pos = camera->GetTransform().GetPosition();
        scene_data->view_matrix = camera->GetViewMatrix();
        scene_data->projection_matrix = camera->GetProjectionMatrix();

        scene_data->point_lights.clear();
        scene_data->directional_lights.clear();
        scene_data->spot_lights.clear();
    }

    void Renderer::AddLight(const PointLightData& light)
    {
        scene_data->point_lights.push_back(light);
    }

    void Renderer::AddLight(const DirectionalLightData& light)
    {
        scene_data->directional_lights.push_back(light);
    }

    void Renderer::AddLight(const SpotLightData& light)
    {
        scene_data->spot_lights.push_back(light);
    }

    void Renderer::EndScene()
    {
    }

    void Renderer::SetViewport(
        uint32_t x,
        uint32_t y,
        uint32_t width,
        uint32_t height)
    {
        render_impl->SetViewport(x, y, width, height);
    }

    void Renderer::SetClearColor(const glm::vec4& color)
    {
        render_impl->SetClearColor(color);
    }

    void Renderer::Clear()
    {
        render_impl->Clear();
    }

    void Renderer::Submit(
        const std::shared_ptr<Shader>& shader,
        const std::shared_ptr<VertexArray>& vertex_array,
        const glm::mat4& model_matrix)
    {
        shader->Bind();
        shader->SetUniformFloat3("u_viewPos", scene_data->view_pos);
        shader->SetUniformMat3("u_NormalMatrix", glm::mat3(glm::transpose(glm::inverse(model_matrix))));
        shader->SetUniformMat4("u_ModelMatrix", model_matrix);
        shader->SetUniformMat4("u_ViewMatrix", scene_data->view_matrix);
        shader->SetUniformMat4("u_ProjectionMatrix", scene_data->projection_matrix);

        shader->SetUniformInt("u_nPointLights", scene_data->point_lights.size());
        for (int i = 0; i < scene_data->point_lights.size(); ++i)
        {
            const auto& light = scene_data->point_lights[i];
            shader->SetUniformFloat3("u_pointLights[" + std::to_string(i) + "].position", light.position);
            shader->SetUniformFloat("u_pointLights[" + std::to_string(i) + "].intensity", light.intensity);
            shader->SetUniformFloat("u_pointLights[" + std::to_string(i) + "].range", light.range);
            shader->SetUniformFloat3("u_pointLights[" + std::to_string(i) + "].color", light.color);
        }

        shader->SetUniformInt("u_nDirectionalLights", scene_data->directional_lights.size());
        for (int i = 0; i < scene_data->directional_lights.size(); ++i)
        {
            const auto& light = scene_data->directional_lights[i];
            shader->SetUniformFloat3("u_directionalLights[" + std::to_string(i) + "].direction", light.direction);
            shader->SetUniformFloat3("u_directionalLights[" + std::to_string(i) + "].color", light.color);
        }

        shader->SetUniformInt("u_nSpotLights", scene_data->spot_lights.size());
        for (int i = 0; i < scene_data->spot_lights.size(); ++i)
        {
            const auto& light = scene_data->spot_lights[i];
            shader->SetUniformFloat3("u_spotLights[" + std::to_string(i) + "].position", light.position);
            shader->SetUniformFloat3("u_spotLights[" + std::to_string(i) + "].direction", light.direction);
            shader->SetUniformFloat("u_spotLights[" + std::to_string(i) + "].cutoff", light.cutoff);
            shader->SetUniformFloat("u_spotLights[" + std::to_string(i) + "].intensity", light.intensity);
            shader->SetUniformFloat("u_spotLights[" + std::to_string(i) + "].range", light.range);
            shader->SetUniformFloat3("u_spotLights[" + std::to_string(i) + "].color", light.color);
        }

        vertex_array->Bind();
        render_impl->DrawTriangles(vertex_array);
    }

}