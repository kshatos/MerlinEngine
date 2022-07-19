#ifndef SKYBOX_HPP
#define SKYBOX_HPP
#include <memory>
#include "Merlin/Render/cubemap.hpp"
#include "Merlin/Render/vertex_array.hpp"
#include "Merlin/Render/shader.hpp"
#include "Merlin/Render/mesh_buffer.hpp"


namespace Merlin
{
    class Skybox
    {
        std::shared_ptr<Cubemap> m_cubemap;
        std::shared_ptr<VertexArray> m_varray;
        std::shared_ptr<MeshBuffer> m_mbuffer;
    public:
        Skybox(const std::shared_ptr<Cubemap>& cubemap, float size);
        inline const std::shared_ptr<Cubemap>& GetCubemap() { return m_cubemap; }
        inline const std::shared_ptr<VertexArray>& GetVertexArray() { return m_varray; }
        inline const std::shared_ptr<MeshBuffer>& GetMeshBuffer() { return m_mbuffer; }
    };
}
#endif