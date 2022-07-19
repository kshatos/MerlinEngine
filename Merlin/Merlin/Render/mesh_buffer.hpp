#ifndef MESH_BUFFER_HPP
#define MESH_BUFFER_HPP
#include "Merlin/Render/index_buffer.hpp"
#include "Merlin/Render/vertex_buffer.hpp"
#include "Merlin/Render/buffer_layout.hpp"


namespace Merlin
{
    class MeshBuffer
    {
        std::shared_ptr<IndexBuffer> m_indexBuffer;
        std::shared_ptr<VertexBuffer> m_vertexBuffer;
    public:
        MeshBuffer(
            std::shared_ptr<IndexBuffer> indexBuffer,
            std::shared_ptr<VertexBuffer> vertexBuffer);

        const std::shared_ptr<IndexBuffer>& GetIndexBuffer();
        const std::shared_ptr<VertexBuffer>& GetVertexBuffer();
        const BufferLayout& GetVertexLayout();
    };
}
#endif