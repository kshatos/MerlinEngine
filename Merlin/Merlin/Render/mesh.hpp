#ifndef MESH_HPP
#define MESH_HPP
#include <stdint.h>

#include <vector>

namespace Merlin
{
    template <typename VertexType>
    class Mesh
    {
        std::vector<VertexType> m_vertices;
        std::vector<uint32_t> m_indices;

    public:
        Mesh() = default;
        Mesh(const VertexType* vertices,
             size_t vertex_count,
             const uint32_t* indices,
             size_t index_count)
        {
            SetVertexData(vertices, vertex_count);
            SetIndexData(indices, index_count);
        }

        inline size_t GetVertexCount() const { return m_vertices.size(); }

        inline size_t GetTriangleCount() const { return m_indices.size() / 3; }

        inline void SetVertexCount(size_t count) { m_vertices.resize(count); }

        inline void SetTriangleCount(size_t count)
        {
            m_indices.resize(count * 3);
        }

        inline VertexType& GetVertex(uint32_t index)
        {
            return m_vertices[index];
        }

        inline uint32_t& GetIndex(uint32_t triangle_index,
                                  uint32_t vertex_offset)
        {
            return m_indices[3 * triangle_index + vertex_offset];
        }

        inline const VertexType& GetVertex(uint32_t index) const
        {
            return m_vertices[index];
        }

        inline const uint32_t& GetIndex(uint32_t triangle_index,
                                        uint32_t vertex_offset) const
        {
            return m_indices[3 * triangle_index + vertex_offset];
        }

        inline void SetVertexData(const VertexType* vertices, size_t count)
        {
            m_vertices.assign(vertices, vertices + count);
        }

        inline void SetIndexData(const uint32_t* indices, size_t count)
        {
            m_indices.assign(indices, indices + count);
        }

        inline float* GetVertexDataPointer()
        {
            return (float*)m_vertices.data();
        }

        inline uint32_t* GetIndexDataPointer() { return m_indices.data(); }
    };
}  // namespace Merlin

#endif