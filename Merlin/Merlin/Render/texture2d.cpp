#include "Merlin/Render/texture2d.hpp"
#include "Merlin/Render/opengl_texture2d.hpp"


namespace Merlin
{

    std::shared_ptr<Texture2D> Texture2D::Create(void* data, size_t size, uint32_t width, uint32_t height)
    {
        return std::make_shared<OpenGLTexture2D>(data, size, width, height);
    }

}