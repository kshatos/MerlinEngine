#include "Merlin/Render/vertex_buffer.hpp"
#include "Merlin/Platform/OpenGL/opengl_vertex_buffer.hpp"


namespace Merlin
{
    std::shared_ptr<VertexBuffer> VertexBuffer::Create(float* vertices, size_t size)
    {
        return std::make_shared<OpenGLVertexBuffer>(vertices, size);
    }
}