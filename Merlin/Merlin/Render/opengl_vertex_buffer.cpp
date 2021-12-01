#include "opengl_vertex_buffer.hpp"
#include "glad/glad.h"


namespace Merlin
{
    OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, size_t size)
    {
        glCreateBuffers(1, &id);
        glBindBuffer(GL_VERTEX_ARRAY, id);
        glBufferData(GL_VERTEX_ARRAY, size, vertices, GL_STATIC_DRAW);

    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        glDeleteBuffers(1, &id);
    }


    void OpenGLVertexBuffer::SetLayout(const BufferLayout& _layout)
    {
        layout = _layout;
    }

    const BufferLayout& OpenGLVertexBuffer::GetLayout() const
    {
        return layout;
    }
    
    void OpenGLVertexBuffer::Bind()
    {
        glBindBuffer(GL_VERTEX_ARRAY, id);
    }

    void OpenGLVertexBuffer::UnBind()
    {
        glBindBuffer(GL_VERTEX_ARRAY, 0);
    }
}
