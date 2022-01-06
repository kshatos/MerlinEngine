#include "Merlin/Render/renderer.hpp"
#include "Merlin/Render/render_api.hpp"
#include "Merlin/Render/shader.hpp"
#include "Merlin/Render/material.hpp"


namespace Merlin
{
    std::unique_ptr<RenderAPI> Renderer::m_render_impl = nullptr;
    std::unique_ptr<Renderer::SceneData> Renderer::m_scene_data = nullptr;

    void Renderer::Init()
    {
        m_render_impl = std::unique_ptr<RenderAPI>(RenderAPI::Create());
        m_render_impl->Init();
        m_scene_data = std::make_unique<Renderer::SceneData>();
    }

    void Renderer::BeginScene(const CameraRenderData& camera)
    {
        m_scene_data->camera = camera;

        m_scene_data->point_lights.clear();
        m_scene_data->directional_lights.clear();
        m_scene_data->spot_lights.clear();
        m_scene_data->mesh_render_data.clear();
    }

    void Renderer::AddLight(const PointLightData& light)
    {
        m_scene_data->point_lights.push_back(light);
    }

    void Renderer::AddLight(const DirectionalLightData& light)
    {
        m_scene_data->directional_lights.push_back(light);
    }

    void Renderer::AddLight(const SpotLightData& light)
    {
        m_scene_data->spot_lights.push_back(light);
    }

    void Renderer::SetAmbientLighting(float ambient_radiance)
    {
        m_scene_data->ambient_radiance = ambient_radiance;
    }

    void Renderer::EndScene()
    {
        for (const auto& item : m_scene_data->mesh_render_data)
        {
            Draw(item);
        }
        if (m_scene_data->skybox)
        {
            Draw(m_scene_data->skybox);
        }
    }

    void Renderer::SetViewport(
        uint32_t x,
        uint32_t y,
        uint32_t width,
        uint32_t height)
    {
        m_render_impl->SetViewport(x, y, width, height);
    }

    void Renderer::SetClearColor(const glm::vec4& color)
    {
        m_render_impl->SetClearColor(color);
    }

    void Renderer::Clear()
    {
        m_render_impl->Clear();
    }

    void Renderer::SetSkybox(const std::shared_ptr<Skybox>& skybox)
    {
        m_scene_data->skybox = skybox;
    }

    void Renderer::Submit(const MeshRenderData& data)
    {
        m_scene_data->mesh_render_data.push_back(data);
    }

    void Renderer::Draw(const std::shared_ptr<Skybox>& skybox)
    {
        auto& cubemap = skybox->GetCubemap();
        auto& varray = skybox->GetVertexArray();
        auto& shader = skybox->GetShader();

        cubemap->Bind(0);

        shader->Bind();
        shader->SetUniformFloat3("u_viewPos", m_scene_data->camera.view_pos);
        shader->SetUniformMat4("u_ViewMatrix", glm::mat4(glm::mat3(m_scene_data->camera.view_matrix)));
        shader->SetUniformMat4("u_ProjectionMatrix", m_scene_data->camera.projection_matrix);

        varray->Bind();

        m_render_impl->DrawTriangles(varray);

        varray->UnBind();
        shader->UnBind();
        cubemap->UnBind(0);
    }

    void Renderer::Draw(const MeshRenderData& data)
    {
        const auto& material = data.material;
        const auto& vertex_array = data.vertex_array;
        const auto& model_matrix = data.model_matrix;

        material->Bind();
        material->SetUniformFloat3("u_viewPos", m_scene_data->camera.view_pos);
        material->SetUniformMat3("u_NormalMatrix", glm::mat3(glm::transpose(glm::inverse(model_matrix))));
        material->SetUniformMat4("u_ModelMatrix", model_matrix);
        material->SetUniformMat4("u_ViewMatrix", m_scene_data->camera.view_matrix);
        material->SetUniformMat4("u_ProjectionMatrix", m_scene_data->camera.projection_matrix);

        material->SetUniformFloat("u_ambientRadiance", m_scene_data->ambient_radiance);

        material->SetUniformInt("u_nPointLights", m_scene_data->point_lights.size());
        for (int i = 0; i < m_scene_data->point_lights.size(); ++i)
        {
            const auto& light = m_scene_data->point_lights[i];
            material->SetUniformFloat3("u_pointLights[" + std::to_string(i) + "].position", light.position);
            material->SetUniformFloat("u_pointLights[" + std::to_string(i) + "].radiantFlux", light.radiantFlux);
            material->SetUniformFloat("u_pointLights[" + std::to_string(i) + "].range", light.range);
            material->SetUniformFloat3("u_pointLights[" + std::to_string(i) + "].color", light.color);
        }

        material->SetUniformInt("u_nDirectionalLights", m_scene_data->directional_lights.size());
        for (int i = 0; i < m_scene_data->directional_lights.size(); ++i)
        {
            const auto& light = m_scene_data->directional_lights[i];
            material->SetUniformFloat3("u_directionalLights[" + std::to_string(i) + "].direction", light.direction);
            material->SetUniformFloat("u_directionalLights[" + std::to_string(i) + "].irradiance", light.irradiance);
            material->SetUniformFloat3("u_directionalLights[" + std::to_string(i) + "].color", light.color);
        }

        material->SetUniformInt("u_nSpotLights", m_scene_data->spot_lights.size());
        for (int i = 0; i < m_scene_data->spot_lights.size(); ++i)
        {
            const auto& light = m_scene_data->spot_lights[i];
            material->SetUniformFloat3("u_spotLights[" + std::to_string(i) + "].position", light.position);
            material->SetUniformFloat3("u_spotLights[" + std::to_string(i) + "].direction", light.direction);
            material->SetUniformFloat("u_spotLights[" + std::to_string(i) + "].cutoffAngle", light.cutoffAngle);
            material->SetUniformFloat("u_spotLights[" + std::to_string(i) + "].falloffRatio", light.falloffRatio);
            material->SetUniformFloat("u_spotLights[" + std::to_string(i) + "].radiantIntensity", light.radiantIntensity);
            material->SetUniformFloat("u_spotLights[" + std::to_string(i) + "].range", light.range);
            material->SetUniformFloat3("u_spotLights[" + std::to_string(i) + "].color", light.color);
        }

        vertex_array->Bind();
        m_render_impl->DrawTriangles(vertex_array);

        vertex_array->UnBind();
        material->UnBind();
    }

}