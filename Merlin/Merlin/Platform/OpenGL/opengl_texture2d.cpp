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

    GLenum GetOpenGLMinFilterMode(TextureFilterMode mode)
    {
        switch (mode)
        {
        case Merlin::TextureFilterMode::Nearest:
            return GL_NEAREST_MIPMAP_NEAREST;
        case Merlin::TextureFilterMode::Linear:
            return GL_LINEAR_MIPMAP_LINEAR;
        default:
            ME_LOG_ERROR("Unkown TextureFilterMode wrap type given");
            return GL_NONE;
        }
    }

    GLenum GetOpenGLMagFilterMode(TextureFilterMode mode)
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

    GLenum GetGLChannelFormat(const uint32_t& channel_count)
    {
        switch (channel_count)
        {
        case 1:
            return GL_RED;
        case 2:
            return GL_RG;
        case 3:
            return GL_RGB;
        case 4:
            return GL_RGBA;
        default:
            ME_LOG_ERROR("Invalid number of texture channels.");
            return GL_NONE;
        }
    }

    void OpenGLTexture2D::Init(
        void* data,
        uint32_t width,
        uint32_t height,
        uint32_t channel_count,
        Texture2DProperties props)
    {
        m_width = width;
        m_height = height;

        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GetOpenGLWrapMode(props.s_wrap_mode));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GetOpenGLWrapMode(props.t_wrap_mode));

        auto min_filter_mode = GetOpenGLMinFilterMode(props.filter_mode);
        auto mag_filter_mode = GetOpenGLMagFilterMode(props.filter_mode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter_mode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter_mode);

        auto gl_channel_format = GetGLChannelFormat(channel_count);
        glTexImage2D(GL_TEXTURE_2D, 0, gl_channel_format, m_width, m_height, 0, gl_channel_format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    OpenGLTexture2D::OpenGLTexture2D(
        Texture2DData texture_data,
        Texture2DProperties props)
    {
        Init(
            texture_data.GetDataPointer(),
            texture_data.GetXResolution(),
            texture_data.GetYResolution(),
            texture_data.GetChannelCount(),
            props);
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
        // Was causing GL_INVALID_OPERATION errors
        //glBindTextureUnit(slot, 0);
    }

}