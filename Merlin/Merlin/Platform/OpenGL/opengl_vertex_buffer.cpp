#include "opengl_vertex_buffer.hpp"
#include "glad/glad.h"


namespace Merlin
{
    OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, size_t size)
    {
        glCreateBuffers(1, &m_id);
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        glDeleteBuffers(1, &m_id);
    }

    void OpenGLVertexBuffer::Bind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
    }

    void OpenGLVertexBuffer::UnBind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}
