#ifndef OPENGL_VERTEX_BUFFER_HPP
#define OPENGL_VERTEX_BUFFER_HPP
#include "vertex_buffer.hpp"
#include <stdint.h>

namespace Merlin
{
    class OpenGLVertexBuffer : public VertexBuffer
    {
        BufferLayout layout;
        uint32_t id;
    public:
        OpenGLVertexBuffer(float* vertices, size_t size);
        ~OpenGLVertexBuffer();
        void SetLayout(const BufferLayout& layout) override;
        const BufferLayout& GetLayout() const override;
        void Bind() override;
        void UnBind() override;
    };
}

#endif