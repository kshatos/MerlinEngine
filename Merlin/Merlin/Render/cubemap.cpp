#include "Merlin/Render/cubemap.hpp"
#include "Merlin/Platform/OpenGL/opengl_cubemap.hpp"


namespace Merlin
{
    std::shared_ptr<Cubemap> Cubemap::Create(const std::vector<std::string>& face_paths)
    {
        return std::make_shared<OpenGLCubemap>(face_paths);
    }
    
    std::shared_ptr<Cubemap> Cubemap::Create(uint32_t resolution, uint32_t channel_count)
    {
        return std::make_shared<OpenGLCubemap>(resolution, channel_count);
    }

}