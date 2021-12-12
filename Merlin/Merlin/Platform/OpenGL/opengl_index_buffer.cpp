#include "Merlin/Platform/OpenGL/opengl_index_buffer.hpp"
#include <glad/glad.h>


namespace Merlin
{
    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t _count) : count(_count)
    {
        glCreateBuffers(1, &id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        glDeleteBuffers(1, &id);
    }

    void OpenGLIndexBuffer::Bind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    }

    void OpenGLIndexBuffer::UnBind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    uint32_t OpenGLIndexBuffer::GetCount()
    {
        return count;
    }

}