#include "opengl_vertex_buffer.hpp"
#include "glad/glad.h"


namespace Merlin
{
    static GLenum GetShaderDataTypeOpenGLType(const ShaderDataType& type)
    {
        switch (type)
        {
        case(ShaderDataType::Float):  return GL_FLOAT;
        case(ShaderDataType::Float2): return GL_FLOAT;
        case(ShaderDataType::Float3): return GL_FLOAT;
        case(ShaderDataType::Float4): return GL_FLOAT;
        case(ShaderDataType::Mat3):   return GL_FLOAT;
        case(ShaderDataType::Mat4):   return GL_FLOAT;
        case(ShaderDataType::Int):    return GL_INT;
        case(ShaderDataType::Int2):   return GL_INT;
        case(ShaderDataType::Int3):   return GL_INT;
        case(ShaderDataType::Int4):   return GL_INT;
        default: return GL_NONE;
        }
    }


    OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, size_t size, BufferLayout layout)
    {
        glCreateBuffers(1, &m_id);
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

        m_layout = layout;
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        glDeleteBuffers(1, &m_id);
    }

    void OpenGLVertexBuffer::Bind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
        uint32_t index = 0;
        for (const auto& element : m_layout)
        {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(
                index,
                ShaderDataTypeElementCount(element.type),
                GetShaderDataTypeOpenGLType(element.type),
                element.normalized ? GL_TRUE : GL_FALSE,
                m_layout.GetStride(),
                (void*)(element.offset));
            index++;
        }
    }

    void OpenGLVertexBuffer::UnBind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}
