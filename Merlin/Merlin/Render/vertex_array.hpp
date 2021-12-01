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
        virtual void Bind() = 0;
        virtual void UnBind() = 0;
        virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& buffer) = 0;
        virtual void AddIndexBuffer(const std::shared_ptr<IndexBuffer>& buffer) = 0;

        static VertexArray* Create();
    };
}
#endif