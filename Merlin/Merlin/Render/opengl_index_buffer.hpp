#ifndef OPENGL_INDEX_BUFFER_HPP
#define OPENGL_INDEX_BUFFER_HPP
#include <stdint.h>
#include "Merlin/Render/index_buffer.hpp"


namespace Merlin
{

    class OpenGLIndexBuffer : public IndexBuffer
    {
        uint32_t count;
        uint32_t id;
    public:
        OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
        ~OpenGLIndexBuffer();
        void Bind() override;
        void UnBind()override;
        uint32_t GetCount() override;
    };
}

#endif