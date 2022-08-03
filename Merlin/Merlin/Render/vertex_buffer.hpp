#ifndef VERTEX_BUFFER_HPP
#define VERTEX_BUFFER_HPP
#include <memory>
#include "Merlin/Render/buffer_data.hpp"

namespace Merlin
{
    class VertexBuffer
    {
    protected:
        BufferLayout m_layout;

    public:
        virtual ~VertexBuffer() {}
        virtual inline const BufferLayout& GetLayout() const { return m_layout; };
    };
}

#endif