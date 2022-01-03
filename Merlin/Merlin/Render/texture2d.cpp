#include "Merlin/Render/texture2d.hpp"
#include "Merlin/Platform/OpenGL/opengl_texture2d.hpp"


namespace Merlin
{

    std::shared_ptr<Texture2D> Texture2D::Create(
        const std::string& filepath,
        Texture2DProperties props)
    {
        return std::make_shared<OpenGLTexture2D>(filepath, props);
    }

    std::shared_ptr<Texture2D> Texture2D::Create(
        void* data,
        uint32_t width,
        uint32_t height,
        uint32_t channel_count,
        Texture2DProperties props)
    {
        return std::make_shared<OpenGLTexture2D>(data, width, height, channel_count, props);
    }

}