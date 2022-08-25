#include "Merlin/Platform/OpenGL/opengl_index_buffer.hpp"

#include <glad/glad.h>

namespace Merlin
{
    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices,
                                         uint32_t index_count)
        : m_index_count(index_count)
    {
        glCreateBuffers(1, &m_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     m_index_count * sizeof(uint32_t),
                     indices,
                     GL_STATIC_DRAW);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer() { glDeleteBuffers(1, &m_id); }

    void OpenGLIndexBuffer::Bind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    }

    void OpenGLIndexBuffer::UnBind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    uint32_t OpenGLIndexBuffer::GetCount() { return m_index_count; }

}  // namespace Merlin