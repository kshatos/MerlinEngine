#include "Merlin/Render/mesh_buffer.hpp"

namespace Merlin
{
    MeshBuffer::MeshBuffer(std::shared_ptr<IndexBuffer> index_buffer,
                           std::shared_ptr<VertexBuffer> vertex_buffer)
        : m_index_buffer(index_buffer), m_vertexBuffer(vertex_buffer)
    {
    }

    const std::shared_ptr<IndexBuffer>& MeshBuffer::GetIndexBuffer()
    {
        return m_index_buffer;
    }

    const std::shared_ptr<VertexBuffer>& MeshBuffer::GetVertexBuffer()
    {
        return m_vertexBuffer;
    }

    const BufferLayout& MeshBuffer::GetVertexLayout()
    {
        return m_vertexBuffer->GetLayout();
    }
}  // namespace Merlin
