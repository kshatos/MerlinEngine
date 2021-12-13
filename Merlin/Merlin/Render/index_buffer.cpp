#include "Merlin/Render/index_buffer.hpp"
#include "Merlin/Platform/OpenGL/opengl_index_buffer.hpp"


namespace Merlin
{
    std::shared_ptr<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
    {
        return std::make_shared<OpenGLIndexBuffer>(indices, count);
    }
}