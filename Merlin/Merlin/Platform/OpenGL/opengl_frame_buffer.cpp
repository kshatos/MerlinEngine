#include <glad/glad.h>
#include "Merlin/Platform/OpenGL/opengl_frame_buffer.hpp"
#include "Merlin/Core/logger.hpp"


namespace Merlin
{

    OpenGLFrameBuffer::OpenGLFrameBuffer(FrameBufferParameters parameters)
    {
        m_parameters = parameters;
        Rebuild();
    }

    OpenGLFrameBuffer::~OpenGLFrameBuffer()
    {
        glDeleteFramebuffers(1, &m_id);
    }

    void OpenGLFrameBuffer::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_id);
        glViewport(0, 0, m_parameters.width, m_parameters.height);
        glEnable(GL_DEPTH_TEST);
    }

    void OpenGLFrameBuffer::UnBind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFrameBuffer::Rebuild()
    {
        glGenFramebuffers(1, &m_id);
        glBindFramebuffer(GL_FRAMEBUFFER, m_id);

        // Color
        glGenTextures(1, &m_color_attachment_id);
        glBindTexture(GL_TEXTURE_2D, m_color_attachment_id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_parameters.width, m_parameters.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_color_attachment_id, 0);

        // Depth
        glGenRenderbuffers(1, &m_depth_attachment_id);
        glBindRenderbuffer(GL_RENDERBUFFER, m_depth_attachment_id);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_parameters.width, m_parameters.height); // use a single renderbuffer object for both a depth AND stencil buffer.
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depth_attachment_id); // now actually attach it

        // Report Status
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE)
        {
            ME_LOG_ERROR("Unable to build frame buffer.");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    }

}
