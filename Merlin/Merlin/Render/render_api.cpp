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
    RenderAPI* RenderAPI::Create(RenderBackend backend)
    {
        switch (backend)
        {
        case RenderBackend::OPENGL:
#ifdef MERLIN_USE_OPENGL
            return new OpenGLRenderAPI();
#else 
            throw std::runtime_error("OpenGL API not avialable!");
            return nullptr;
#endif
            break;
        case RenderBackend::VULKAN:
#ifdef MERLIN_USE_VULKAN
            return new VulkanRenderAPI();
#else 
            throw std::runtime_error("Vulkan API not avialable!");
            return nullptr;
#endif
            break;
        }
    }

}