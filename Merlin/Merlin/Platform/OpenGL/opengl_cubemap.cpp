#include<glad/glad.h>
#include <stb/stb_image.h>
#include "Merlin/Platform/OpenGL/opengl_cubemap.hpp"
#include"Merlin/Core/logger.hpp"


namespace Merlin
{
    OpenGLCubemap::OpenGLCubemap(const std::vector<std::string>& face_paths)
    {
        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);

        int width, height, nrChannels;
        for (unsigned int i = 0; i < face_paths.size(); i++)
        {
            unsigned char* data = stbi_load(face_paths[i].c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            }
            else
            {
                ME_LOG_ERROR("Cubemap texture failed to load at path: " + face_paths[i]);
            }
            stbi_image_free(data);
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
    
    void OpenGLCubemap::Bind()
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);
    }

    void OpenGLCubemap::UnBind()
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }
}