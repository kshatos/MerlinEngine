#ifndef OPENGL_VERTEX_BUFFER_HPP
#define OPENGL_VERTEX_BUFFER_HPP
#include "Merlin/Render/vertex_buffer.hpp"
#include <stdint.h>

namespace Merlin
{
    class OpenGLVertexBuffer : public VertexBuffer
    {
        BufferLayout m_layout;
        uint32_t m_id;
    public:
        OpenGLVertexBuffer(float* vertices, size_t size);
        ~OpenGLVertexBuffer();
        void SetLayout(const BufferLayout& layout) override;
        const BufferLayout& GetLayout() const override;
        void Bind();
        void UnBind();
    };
}

#endif