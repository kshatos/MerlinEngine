#include <glad/glad.h>
#include "Merlin/Render/opengl_frame_buffer.hpp"
#include "Merlin/Core/logger.hpp"


namespace Merlin
{

    OpenGLFrameBuffer::OpenGLFrameBuffer(FrameBufferParameters _params)
    {
        params = _params;
        Rebuild();
    }

    OpenGLFrameBuffer::~OpenGLFrameBuffer()
    {
        glDeleteFramebuffers(1, &id);
    }

    void OpenGLFrameBuffer::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, id);
        glViewport(0, 0, params.width, params.height);
        glEnable(GL_DEPTH_TEST);
    }

    void OpenGLFrameBuffer::UnBind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFrameBuffer::Rebuild()
    {
        glGenFramebuffers(1, &id);
        glBindFramebuffer(GL_FRAMEBUFFER, id);

        // Color
        glGenTextures(1, &color_attachment_id);
        glBindTexture(GL_TEXTURE_2D, color_attachment_id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, params.width, params.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_attachment_id, 0);

        // Depth
        glGenRenderbuffers(1, &depth_attachment_id);
        glBindRenderbuffer(GL_RENDERBUFFER, depth_attachment_id);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, params.width, params.height); // use a single renderbuffer object for both a depth AND stencil buffer.
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth_attachment_id); // now actually attach it

        // Report Status
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE)
        {
            ME_LOG_ERROR("Unable to build frame buffer.");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    }

}
