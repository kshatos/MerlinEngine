#ifndef OPENGL_VERTEX_ARRAY_HPP
#define OPENGL_VERTEX_ARRAY_HPP
#include <stdint.h>
#include <vector>
#include "Merlin/Render/vertex_array.hpp"


namespace Merlin
{
    class OpenGLVertexArray : public VertexArray
    {
        uint32_t m_id;
        std::vector<std::shared_ptr<VertexBuffer>> m_vertex_buffers;
        std::shared_ptr<IndexBuffer> m_index_buffer;
    public:
        OpenGLVertexArray();
        ~OpenGLVertexArray();
        void Bind();
        void UnBind();
        void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& buffer) override;
        void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& buffer) override;
        const std::shared_ptr<IndexBuffer>& GetIndexBuffer() override;
    };
}
#endif