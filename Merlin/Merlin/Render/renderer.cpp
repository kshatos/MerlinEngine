#include "Merlin/Render/renderer.hpp"
#include "Merlin/Render/render_api.hpp"
#include "Merlin/Render/shader.hpp"
#include "Merlin/Render/material.hpp"


namespace Merlin
{
    std::shared_ptr<Shader> Renderer::m_shadow_shader = nullptr;
    std::unique_ptr<RenderAPI> Renderer::m_render_impl = nullptr;

    void Renderer::RenderScene(const SceneRenderData& scene)
    {
        if (scene.camera == nullptr)
            return;
        scene.camera->frame_buffer->Bind();
        m_render_impl->SetClearColor(scene.camera->clear_color);
        m_render_impl->Clear();
        DrawMeshes(scene);
        DrawSkybox(scene);
        scene.camera->frame_buffer->UnBind();
    }

    void Renderer::Init()
    {
        m_render_impl = std::unique_ptr<RenderAPI>(RenderAPI::Create());
        m_render_impl->Init();
        m_shadow_shader = Shader::CreateFromFiles(
            ".\\Assets\\Shaders\\shadow.vert",
            ".\\Assets\\Shaders\\shadow.frag");
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

    void Renderer::DrawMeshes(const SceneRenderData& scene)
    {
        auto& camera = *scene.camera;
        for (const auto& mesh_pointer : scene.meshes)
        {
            MeshRenderData& data = *mesh_pointer;

            const auto& material = data.material;
            const auto& vertex_array = data.vertex_array;
            const auto& model_matrix = data.model_matrix;

            material->Bind();
            material->SetUniformFloat3("u_viewPos", camera.view_pos);
            material->SetUniformMat3("u_NormalMatrix", glm::mat3(glm::transpose(glm::inverse(model_matrix))));
            material->SetUniformMat4("u_ModelMatrix", model_matrix);
            material->SetUniformMat4("u_ViewMatrix", camera.view_matrix);
            material->SetUniformMat4("u_ProjectionMatrix", camera.projection_matrix);

            material->SetUniformFloat("u_ambientRadiance", scene.ambient_light_radiance);

            material->SetUniformInt("u_nPointLights", scene.point_lights.size());
            for (int i = 0; i < scene.point_lights.size(); ++i)
            {
                const auto& light = *scene.point_lights[i];
                material->SetUniformFloat3("u_pointLights[" + std::to_string(i) + "].position", light.position);
                material->SetUniformFloat("u_pointLights[" + std::to_string(i) + "].radiantFlux", light.radiantFlux);
                material->SetUniformFloat("u_pointLights[" + std::to_string(i) + "].range", light.range);
                material->SetUniformFloat3("u_pointLights[" + std::to_string(i) + "].color", light.color);
            }

            material->SetUniformInt("u_nDirectionalLights", scene.directional_lights.size());
            for (int i = 0; i < scene.directional_lights.size(); ++i)
            {
                const auto& light = *scene.directional_lights[i];
                material->SetUniformFloat3("u_directionalLights[" + std::to_string(i) + "].direction", light.direction);
                material->SetUniformFloat("u_directionalLights[" + std::to_string(i) + "].irradiance", light.irradiance);
                material->SetUniformFloat3("u_directionalLights[" + std::to_string(i) + "].color", light.color);
            }

            material->SetUniformInt("u_nSpotLights", scene.spot_lights.size());
            for (int i = 0; i < scene.spot_lights.size(); ++i)
            {
                const auto& light = *scene.spot_lights[i];
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

    void Renderer::DrawSkybox(const SceneRenderData& scene)
    {
        auto& camera = *scene.camera;
        auto skybox = camera.skybox;
        if (skybox)
        {
            auto& cubemap = skybox->GetCubemap();
            auto& varray = skybox->GetVertexArray();
            auto& shader = skybox->GetShader();

            cubemap->Bind(0);

            shader->Bind();
            shader->SetUniformFloat3("u_viewPos", camera.view_pos);
            shader->SetUniformMat4("u_ViewMatrix", glm::mat4(glm::mat3(camera.view_matrix)));
            shader->SetUniformMat4("u_ProjectionMatrix", camera.projection_matrix);

            varray->Bind();

            m_render_impl->DrawTriangles(varray);

            varray->UnBind();
            shader->UnBind();
            cubemap->UnBind(0);
        }
    }

}