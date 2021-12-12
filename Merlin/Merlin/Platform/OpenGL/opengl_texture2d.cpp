#include "Merlin/Platform/OpenGL/opengl_texture2d.hpp"
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "Merlin/Core/logger.hpp"


namespace Merlin
{
    GLenum GetOpenGLWrapMode(TextureWrapMode mode)
    {
        switch (mode)
        {
        case Merlin::TextureWrapMode::Repeat:
            return GL_REPEAT;
        case Merlin::TextureWrapMode::MirroredRepeat:
            return GL_MIRRORED_REPEAT;
        case Merlin::TextureWrapMode::ClampToEdge:
            return GL_CLAMP_TO_EDGE;
        case Merlin::TextureWrapMode::ClampToBorder:
            return GL_CLAMP_TO_BORDER;
        default:
            ME_LOG_ERROR("Unkown TextureWrapMode given");
            return GL_NONE;
        }
    }

    GLenum GetOpenGLFilterMode(TextureFilterMode mode)
    {
        switch (mode)
        {
        case Merlin::TextureFilterMode::Nearest:
            return GL_NEAREST;
        case Merlin::TextureFilterMode::Linear:
            return GL_LINEAR;
        default:
            ME_LOG_ERROR("Unkown TextureFilterMode wrap type given");
            return GL_NONE;
        }
    }

    void OpenGLTexture2D::Init(
        void* data,
        uint32_t width,
        uint32_t height,
        Texture2DProperties props)
    {
        m_width = width;
        m_height = height;

        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GetOpenGLWrapMode(props.s_wrap_mode));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GetOpenGLWrapMode(props.t_wrap_mode));

        auto filter_mode = GetOpenGLFilterMode(props.filter_mode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_mode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_mode);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    OpenGLTexture2D::OpenGLTexture2D(
        const std::string& filepath,
        Texture2DProperties props)
    {
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(1);
        unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            Init(data, width, height, props);
        }
        else
        {
            ME_LOG_ERROR("Failed to load texture at: " + filepath);
        }
        stbi_image_free(data);
    }

    OpenGLTexture2D::OpenGLTexture2D(
        void* data,
        uint32_t width,
        uint32_t height,
        Texture2DProperties props)
    {
        Init(data, width, height, props);
    }

    OpenGLTexture2D::~OpenGLTexture2D()
    {
        glDeleteTextures(1, &m_id);
    }
    
    void OpenGLTexture2D::Bind(uint32_t slot)
    {
        glBindTextureUnit(slot, m_id);
    }
    
    void OpenGLTexture2D::UnBind(uint32_t slot)
    {
        glBindTextureUnit(slot, 0);
    }

}