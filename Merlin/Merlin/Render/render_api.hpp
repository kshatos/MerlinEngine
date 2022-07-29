#ifndef RENDER_API_HPP
#define RENDER_API_HPP
#include <glm/glm.hpp>
#include "Merlin/Render/skybox.hpp"
#include "Merlin/Render/vertex_buffer.hpp"
#include "Merlin/Render/index_buffer.hpp"
#include "Merlin/Render/mesh_buffer.hpp"
#include "Merlin/Render/shader.hpp"
#include "Merlin/Render/texture2d.hpp"
#include "Merlin/Render/texture2d_data.hpp"
#include "Merlin/Render/frame_buffer.hpp"
#include "Merlin/Render/scene_render_data.hpp"


namespace Merlin
{
    enum class RenderBackend
    {
        OPENGL,
        VULKAN
    };

    class RenderAPI
    {
    protected:
        std::shared_ptr<Shader> m_shadow_shader;
        std::shared_ptr<Shader> m_skybox_shader;
        std::shared_ptr<FrameBuffer> m_shadow_buffer;

    public:
        virtual ~RenderAPI() {}
        virtual void Init(void* windowPointer) = 0;
        virtual void Shutdown() = 0;

        virtual void BeginFrame() = 0;
        virtual void RenderScene(const SceneRenderData& scene) = 0;
        virtual void EndFrame() = 0;
        virtual void PresentFrame() = 0;

        virtual RenderBackend Backend() = 0;

        virtual std::shared_ptr<MeshBuffer> CreateMeshBuffer(
            float* vertices,
            size_t vertex_count,
            uint32_t* indices,
            size_t index_count,
            BufferLayout vertexLayout) = 0;
        virtual std::shared_ptr<Material> CreateMaterial(MaterialInfo info) = 0;
        virtual std::shared_ptr<MaterialInstance> CreateMaterialInstance(
            std::shared_ptr<Material> material) = 0;
        virtual std::shared_ptr<Shader> CreateShader(
            const std::string& vertex_source,
            const std::string& fragment_source) = 0;
        virtual std::shared_ptr<Texture2D> CreateTexture2D(
            const Texture2DData& texture_data,
            Texture2DProperties props = Texture2DProperties()) = 0;
        virtual std::shared_ptr<Cubemap> CreateCubemap(
            const std::vector<std::string>& face_paths) = 0;
        virtual std::shared_ptr<Cubemap> CreateCubemap(
            uint32_t resolution, uint32_t channel_count) = 0;
        virtual std::shared_ptr<FrameBuffer> CreateFramebuffer(
            const FrameBufferParameters& state) = 0;

        static std::shared_ptr<RenderAPI> Create(RenderBackend backend);
    };

    glm::mat4 GetLightMatrix(
        const CameraRenderData& camera_data,
        const DirectionalLightData& light_data);
}

#endif