#ifndef OPENGL_RENDER_API_HPP
#define OPENGL_RENDER_API_HPP
#include "Merlin/Render/render_api.hpp"
#include <GLFW/glfw3.h>
#include <imgui.h>


namespace Merlin
{
    class OpenGLRenderAPI : public RenderAPI
    {
        GLFWwindow* m_window;
        ImGuiContext* context;
    public:
        void Init(void* windowPointer) override;
        void Shutdown() override;
        
        void BeginFrame() override;
        void EndFrame() override;
        void PresentFrame() override;
        
        void RenderScene(const SceneRenderData& scene) override;
        RenderBackend Backend() override;

        std::shared_ptr<MeshBuffer> CreateMeshBuffer(
            float* vertices,
            size_t vertex_count,
            uint32_t* indices,
            size_t index_count,
            BufferLayout vertexLayout) override;
        std::shared_ptr<Material> CreateMaterial(
            MaterialInfo info) override;
        std::shared_ptr<MaterialInstance> CreateMaterialInstance(
            std::shared_ptr<Material> material) override;
        std::shared_ptr<Shader> CreateShader(
            const std::string& vertex_source,
            const std::string& fragment_source) override;
        std::shared_ptr<Texture2D> CreateTexture2D(
            const Texture2DData& texture_data,
            Texture2DProperties props = Texture2DProperties()) override;
        std::shared_ptr<Cubemap> CreateCubemap(
            const std::vector<std::string>& face_paths) override;
        std::shared_ptr<Cubemap> CreateCubemap(
            uint32_t resolution, uint32_t channel_count) override;
        std::shared_ptr<FrameBuffer> CreateFramebuffer(
            const FrameBufferParameters& state) override;

    private:
        void BindMaterial(const std::shared_ptr<MaterialInstance>& material_instance);
        void DrawMeshes(const SceneRenderData& scene);
        void DrawSkybox(const SceneRenderData& scene);
        void DrawMeshShadows(const SceneRenderData& scene);
    };
}

#endif