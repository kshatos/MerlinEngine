#ifndef VERTEX_ARRAY_HPP
#define VERTEX_ARRAY_HPP
#include <memory>
#include "Merlin/Render/vertex_buffer.hpp"
#include "Merlin/Render/index_buffer.hpp"


namespace Merlin
{
    class VertexArray
    {
    public:
        virtual ~VertexArray() {};
        virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& buffer) = 0;
        virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& buffer) = 0;
        virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() = 0;
    };
}
#endif