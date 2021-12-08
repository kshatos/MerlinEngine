#include "Merlin/Render/opengl_texture2d.hpp"
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "Merlin/Core/logger.hpp"


namespace Merlin
{
    void OpenGLTexture2D::Init(void* data, uint32_t _width, uint32_t _height)
    {
        width = _width;
        height = _height;

        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    OpenGLTexture2D::OpenGLTexture2D(const std::string& filepath)
    {
        int width, height, nrChannels;
        unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            Init(data, width, height);
        }
        else
        {
            ME_LOG_ERROR("Failed to load texture at: " + filepath);
        }
        stbi_image_free(data);
    }

    OpenGLTexture2D::OpenGLTexture2D(void* data, uint32_t _width, uint32_t _height)
    {
        Init(data, _width, _height);
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