#include "Merlin/Platform/OpenGL/opengl_vertex_array.hpp"
#include "Merlin/Platform/OpenGL/opengl_vertex_buffer.hpp"
#include "Merlin/Platform/OpenGL/opengl_index_buffer.hpp"
#include <glad/glad.h>


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

    OpenGLVertexArray::OpenGLVertexArray()
    {
        glCreateVertexArrays(1, &m_id);
    }

    OpenGLVertexArray::~OpenGLVertexArray()
    {
        glDeleteVertexArrays(1, &m_id);
    }

    void OpenGLVertexArray::Bind()
    {
        glBindVertexArray(m_id);
    }

    void OpenGLVertexArray::UnBind()
    {
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& buffer)
    {
        auto gl_buffer = std::dynamic_pointer_cast<OpenGLVertexBuffer>(buffer);
        m_vertex_buffers.push_back(buffer);

        glBindVertexArray(m_id);
        gl_buffer->Bind();
        glBindVertexArray(0);
        gl_buffer->UnBind();
    }

    void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& buffer)
    {
        auto gl_buffer = std::dynamic_pointer_cast<OpenGLIndexBuffer>(buffer);
        m_index_buffer = buffer;

        glBindVertexArray(m_id);
        gl_buffer->Bind();
        glBindVertexArray(0);
        gl_buffer->UnBind();
    }

    const std::shared_ptr<IndexBuffer>& OpenGLVertexArray::GetIndexBuffer()
    {
        return m_index_buffer;
    }
}
