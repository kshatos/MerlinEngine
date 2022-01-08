#include <glad/glad.h>
#include "Merlin/Platform/OpenGL/opengl_frame_buffer.hpp"
#include "Merlin/Core/logger.hpp"


namespace Merlin
{
    GLenum GetGLColorBufferFormat(ColorBufferFormat format)
    {
        switch (format)
        {
        case ColorBufferFormat::RGBA8:
            return GL_RGBA;
        default:
            return GL_NONE;
        }
    }

    GLenum GetGLDepthBufferInternalFormat(DepthBufferFormat format)
    {
        switch (format)
        {
        case DepthBufferFormat::DEPTH32:
            return GL_DEPTH_COMPONENT32;
        case DepthBufferFormat::DEPTH24_STENCIL8:
            return GL_DEPTH24_STENCIL8;
        default:
            return GL_NONE;
        }
    }

    GLenum GetGLDepthBufferFormat(DepthBufferFormat format)
    {
        switch (format)
        {
        case DepthBufferFormat::DEPTH32:
            return GL_DEPTH_COMPONENT;
        case DepthBufferFormat::DEPTH24_STENCIL8:
            return GL_DEPTH_STENCIL;
        default:
            return GL_NONE;
        }
    }

    GLenum GetDepthDataType(DepthBufferFormat format)
    {
        switch (format)
        {
        case DepthBufferFormat::DEPTH32:
            return GL_UNSIGNED_INT;
        case DepthBufferFormat::DEPTH24_STENCIL8:
            return GL_UNSIGNED_INT_24_8;
        default:
            return GL_NONE;
        }
    }

    GLenum GetDepthAttachmentType(DepthBufferFormat format)
    {
        switch (format)
        {
        case Merlin::DepthBufferFormat::DEPTH32:
            return GL_DEPTH_ATTACHMENT;
        case Merlin::DepthBufferFormat::DEPTH24_STENCIL8:
            return GL_DEPTH_STENCIL_ATTACHMENT;
        default:
            return GL_NONE;
        }
    }


    OpenGLFrameBuffer::OpenGLFrameBuffer(FrameBufferParameters parameters) :
        m_parameters(parameters),
        m_color_attachment_id(0),
        m_depth_attachment_id(0)
    {
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
        if (m_parameters.color_buffer_format != ColorBufferFormat::NONE)
        {
            auto gl_format = GetGLColorBufferFormat(m_parameters.color_buffer_format);
            glGenTextures(1, &m_color_attachment_id);
            glBindTexture(GL_TEXTURE_2D, m_color_attachment_id);
            glTexImage2D(GL_TEXTURE_2D, 0, gl_format, m_parameters.width, m_parameters.height, 0, gl_format, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_color_attachment_id, 0);
        }

        // Depth
        if (m_parameters.depth_buffer_format != DepthBufferFormat::NONE)
        {
            auto gl_internal_format = GetGLDepthBufferInternalFormat(m_parameters.depth_buffer_format);
            auto gl_format = GetGLDepthBufferFormat(m_parameters.depth_buffer_format);
            auto gl_type = GetDepthDataType(m_parameters.depth_buffer_format);
            auto gl_attachment_type = GetDepthAttachmentType(m_parameters.depth_buffer_format);

            glGenTextures(1, &m_depth_attachment_id);
            glBindTexture(GL_TEXTURE_2D, m_depth_attachment_id);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexImage2D(GL_TEXTURE_2D, 0, gl_internal_format, m_parameters.width, m_parameters.height, 0, gl_format, gl_type, NULL);
            glFramebufferTexture2D(GL_FRAMEBUFFER, gl_attachment_type, GL_TEXTURE_2D, m_depth_attachment_id, 0);
        }

        // Report Status
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE)
        {
            ME_LOG_ERROR("Unable to build frame buffer.");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

}
