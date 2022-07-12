#ifndef OPENGL_INDEX_BUFFER_HPP
#define OPENGL_INDEX_BUFFER_HPP
#include <stdint.h>
#include "Merlin/Render/index_buffer.hpp"


namespace Merlin
{

    class OpenGLIndexBuffer : public IndexBuffer
    {
        uint32_t m_index_count;
        uint32_t m_id;
    public:
        OpenGLIndexBuffer(uint32_t* indices, uint32_t index_count);
        ~OpenGLIndexBuffer();
        void Bind();
        void UnBind();
        uint32_t GetCount() override;
    };
}

#endif