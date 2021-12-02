#ifndef VERTEX_BUFFER_HPP
#define VERTEX_BUFFER_HPP
#include "Merlin/Render/buffer_layout.hpp"


namespace Merlin
{
    class VertexBuffer
    {
    public:
        static VertexBuffer* Create(float* vertices, size_t size);

        virtual ~VertexBuffer() {}
        virtual void SetLayout(const BufferLayout& layout) = 0;
        virtual const BufferLayout& GetLayout() const = 0;
        virtual void Bind() = 0;
        virtual void UnBind() = 0;
    };
}

#endif