#include "Merlin/Render/opengl_vertex_array.hpp"
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
        glCreateVertexArrays(1, &id);
    }

    OpenGLVertexArray::~OpenGLVertexArray()
    {
        glDeleteVertexArrays(1, &id);
    }

    void OpenGLVertexArray::Bind()
    {
        glBindVertexArray(id);
    }

    void OpenGLVertexArray::UnBind()
    {
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& buffer)
    {
        vertex_buffers.push_back(buffer);

        glBindVertexArray(id);
        buffer->Bind();

        const auto& layout = buffer->GetLayout();
        uint32_t index = 0;
        for (const auto& element : layout)
        {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(
                index,
                ShaderDataTypeElementCount(element.type),
                GetShaderDataTypeOpenGLType(element.type),
                element.normalized ? GL_TRUE : GL_FALSE,
                layout.GetStride(),
                (void*)(element.offset));
            index++;
        }
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& buffer)
    {
        index_buffer = buffer;

        glBindVertexArray(id);
        buffer->Bind();
    }

    const std::shared_ptr<IndexBuffer>& OpenGLVertexArray::GetIndexBuffer()
    {
        return index_buffer;
    }
}
