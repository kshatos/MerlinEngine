#include "Merlin/Render/index_buffer.hpp"
#include "Merlin/Render/opengl_index_buffer.hpp"


namespace Merlin
{
    IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
    {
        return new OpenGLIndexBuffer(indices, count);
    }
}