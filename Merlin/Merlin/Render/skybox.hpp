#ifndef SKYBOX_HPP
#define SKYBOX_HPP
#include <memory>
#include "Merlin/Render/cubemap.hpp"
#include "Merlin/Render/vertex_array.hpp"
#include "Merlin/Render/shader.hpp"


namespace Merlin
{
    class Skybox
    {
        std::shared_ptr<Cubemap> m_cubemap;
        std::shared_ptr<VertexArray> m_varray;
    public:
        Skybox(const std::shared_ptr<Cubemap>& cubemap, float size);
        inline const std::shared_ptr<Cubemap>& GetCubemap() { return m_cubemap; }
        inline const std::shared_ptr<VertexArray>& GetVertexArray() { return m_varray; }
    };
}
#endif