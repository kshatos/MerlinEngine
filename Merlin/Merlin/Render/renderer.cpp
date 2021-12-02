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
        scene_data = std::make_unique<Renderer::SceneData>();
    }

    void Renderer::BeginScene(const std::shared_ptr<Camera>& camera)
    {
        scene_data->view_matrix = camera->GetViewMatrix();
        scene_data->projection_matrix = camera->GetProjectionMatrix();
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
        const std::shared_ptr<VertexArray>& vertex_array)
    {
        shader->Bind();
        shader->SetUniformMat4("u_ProjectionMatrix", scene_data->projection_matrix);
        shader->SetUniformMat4("u_ViewMatrix", scene_data->view_matrix);

        vertex_array->Bind();
        render_impl->DrawTriangles(vertex_array);
    }

}