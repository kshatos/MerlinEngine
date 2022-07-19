#include "Merlin/Render/mesh_buffer.hpp"


namespace Merlin
{
    MeshBuffer::MeshBuffer(
        std::shared_ptr<IndexBuffer> indexBuffer,
        std::shared_ptr<VertexBuffer> vertexBuffer,
        BufferLayout vertexLayout) :
        m_indexBuffer(indexBuffer),
        m_vertexBuffer(vertexBuffer),
        m_vertexLayout(vertexLayout)
    {
    }

    const std::shared_ptr<IndexBuffer>& MeshBuffer::GetIndexBuffer()
    {
        return m_indexBuffer;
    }

    const std::shared_ptr<VertexBuffer>& MeshBuffer::GetVertexBuffer()
    {
        return m_vertexBuffer;
    }

    const BufferLayout& MeshBuffer::GetVertexLayout()
    {
        return m_vertexLayout;
    }
}
