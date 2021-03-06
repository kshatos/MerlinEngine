#ifndef OPENGL_VERTEX_BUFFER_HPP
#define OPENGL_VERTEX_BUFFER_HPP
#include "Merlin/Render/vertex_buffer.hpp"
#include <stdint.h>

namespace Merlin
{
    class OpenGLVertexBuffer : public VertexBuffer
    {
        uint32_t m_id;
    public:
        OpenGLVertexBuffer(
            float* vertices,
            size_t size,
            BufferLayout layout);
        ~OpenGLVertexBuffer();
        void Bind();
        void UnBind();
    };
}

#endif