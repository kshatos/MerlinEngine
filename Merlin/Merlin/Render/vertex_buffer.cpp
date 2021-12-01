#include "vertex_buffer.hpp"
#include "opengl_vertex_buffer.hpp"


namespace Merlin
{
    VertexBuffer* VertexBuffer::Create(float* vertices, size_t size)
    {
        return new OpenGLVertexBuffer(vertices, size);
    }
}