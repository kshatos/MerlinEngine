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
        std::shared_ptr<Shader> m_shader;
    public:
        Skybox(const std::shared_ptr<Cubemap>& cubemap, float size);
        inline void SetShader(std::shared_ptr<Shader> shader) { m_shader = shader; }
        inline const std::shared_ptr<Cubemap>& GetCubemap() { return m_cubemap; }
        inline const std::shared_ptr<VertexArray>& GetVertexArray() { return m_varray; }
        inline const std::shared_ptr<Shader>& GetShader() { return m_shader; }
    };
}
#endif