#include<glad/glad.h>
#include <stb/stb_image.h>
#include "Merlin/Platform/OpenGL/opengl_cubemap.hpp"
#include"Merlin/Core/logger.hpp"


namespace Merlin
{
    GLenum GetGLDataFormat(uint32_t channel_count)
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
            return GL_NONE;
        }
    }

    OpenGLCubemap::OpenGLCubemap(const std::vector<std::string>& face_paths)
    {
        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);

        int width, height, channel_count;
        for (unsigned int i = 0; i < face_paths.size(); i++)
        {
            unsigned char* data = stbi_load(face_paths[i].c_str(), &width, &height, &channel_count, 0);
            if (data)
            {
                auto format = GetGLDataFormat(channel_count);
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            }
            else
            {
                ME_LOG_ERROR("Cubemap texture failed to load at path: " + face_paths[i]);
            }
            stbi_image_free(data);
        }
        m_channel_count = channel_count;
        m_resolution = width;

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }

    OpenGLCubemap::OpenGLCubemap(uint32_t resolution, uint32_t channel_count) :
        m_resolution(resolution),
        m_channel_count(channel_count)
    {
        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);

        auto format = GetGLDataFormat(channel_count);

        for (unsigned int i = 0; i < 6; i++)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, resolution, resolution, 0, format, GL_UNSIGNED_BYTE, nullptr);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }

    OpenGLCubemap::~OpenGLCubemap()
    {
        glDeleteTextures(1, &m_id);
    }

    void OpenGLCubemap::Bind(uint32_t slot)
    {
        glBindTextureUnit(slot, m_id);
    }

    void OpenGLCubemap::UnBind(uint32_t slot)
    {
        glBindTextureUnit(slot, 0);
    }

    void OpenGLCubemap::SetFaceData(CubeFace face, float* data)
    {
        auto format = GetGLDataFormat(m_channel_count);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, format, m_resolution, m_resolution, 0, format, GL_FLOAT, data);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }

}