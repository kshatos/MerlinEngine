#include "Merlin/Render/opengl_texture2d.hpp"
#include <glad/glad.h>


namespace Merlin
{
    OpenGLTexture2D::OpenGLTexture2D(void* data, size_t size, uint32_t _width, uint32_t _height)
    {
        width = _width;
        height = _height;

        glCreateTextures(GL_TEXTURE_2D, 1, &id);
        glTextureStorage2D(id, 1, GL_RGB8, width, height);
        glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureSubImage2D(id, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
    }

    OpenGLTexture2D::~OpenGLTexture2D()
    {
        glDeleteTextures(1, &id);
    }
    
    void OpenGLTexture2D::Bind(uint32_t slot)
    {
        glBindTextureUnit(slot, id);
    }
    
    void OpenGLTexture2D::UnBind(uint32_t slot)
    {
        glBindTextureUnit(slot, 0);
    }

}