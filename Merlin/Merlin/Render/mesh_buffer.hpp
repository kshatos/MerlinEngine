#ifndef MESH_BUFFER_HPP
#define MESH_BUFFER_HPP
#include "Merlin/Render/buffer_data.hpp"
#include "Merlin/Render/index_buffer.hpp"
#include "Merlin/Render/vertex_buffer.hpp"

namespace Merlin
{
    class MeshBuffer
    {
        std::shared_ptr<IndexBuffer> m_index_buffer;
        std::shared_ptr<VertexBuffer> m_vertexBuffer;

    public:
        MeshBuffer(std::shared_ptr<IndexBuffer> index_buffer,
                   std::shared_ptr<VertexBuffer> vertex_buffer);

        const std::shared_ptr<IndexBuffer>& GetIndexBuffer();
        const std::shared_ptr<VertexBuffer>& GetVertexBuffer();
        const BufferLayout& GetVertexLayout();
    };
}  // namespace Merlin
#endif