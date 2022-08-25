#include "Merlin/Render/render_api.hpp"
#ifdef MERLIN_USE_OPENGL
#include "Merlin/Platform/OpenGL/opengl_render_api.hpp"
#endif
#ifdef MERLIN_USE_VULKAN
#include "Merlin/Platform/Vulkan/vulkan_render_api.hpp"
#endif
#include <stdexcept>

namespace Merlin
{
    std::shared_ptr<RenderAPI> RenderAPI::Create(RenderBackend backend)
    {
        switch (backend)
        {
            case RenderBackend::OpenGL:
#ifdef MERLIN_USE_OPENGL
                return std::make_shared<OpenGLRenderAPI>();
#else
                throw std::runtime_error("OpenGL API not avialable!");
                return nullptr;
#endif
                break;
            case RenderBackend::Vulkan:
#ifdef MERLIN_USE_VULKAN
                return std::make_shared<VulkanRenderAPI>();
#else
                throw std::runtime_error("Vulkan API not avialable!");
                return nullptr;
#endif
                break;
        }
        return nullptr;
    }

    glm::mat4 GetLightMatrix(const CameraRenderData& camera_data,
                             const DirectionalLightData& light_data)
    {
        auto distance = 10.0f;
        auto projection_matrix = glm::ortho(
            -distance, +distance, -distance, +distance, +00.01f, +2 * distance);

        Transform t;
        t.LookAt(light_data.direction);
        t.Translate(camera_data.view_pos - light_data.direction * distance);
        auto view_matrix = glm::inverse(t.GetTransformationMatrix());

        return projection_matrix * view_matrix;
    }
}  // namespace Merlin