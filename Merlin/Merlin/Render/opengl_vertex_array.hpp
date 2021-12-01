#ifndef OPENGL_VERTEX_ARRAY_HPP
#define OPENGL_VERTEX_ARRAY_HPP
#include <stdint.h>
#include <vector>
#include "Merlin/Render/vertex_array.hpp"


namespace Merlin
{
    class OpenGLVertexArray : public VertexArray
    {
        uint32_t id;
        std::vector<std::shared_ptr<VertexBuffer>> vertex_buffers;
        std::shared_ptr<IndexBuffer> index_buffer;
    public:
        OpenGLVertexArray();
        ~OpenGLVertexArray();
        void Bind() override;
        void UnBind() override;
        void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& buffer) override;
        void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& buffer) override;
        const std::shared_ptr<IndexBuffer>& GetIndexBuffer() override;
    };
}
#endif