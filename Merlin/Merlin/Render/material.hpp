#ifndef MATERIAL_HPP
#define MATERIAL_HPP
#include <string>
#include <vector>

#include "Merlin/Render/buffer_data.hpp"
#include "Merlin/Render/shader.hpp"
#include "Merlin/Render/texture2d.hpp"

namespace Merlin
{
    struct MaterialInfo
    {
        std::shared_ptr<Shader> shader;
        BufferLayout uniform_layout;
        std::vector<std::string> texture_names;

        MaterialInfo(std::shared_ptr<Shader> _shader,
                     BufferLayout _uniformLayout,
                     std::vector<std::string> _textureNames)
            : shader(_shader)
            , uniform_layout(_uniformLayout)
            , texture_names(_textureNames)
        {
        }
    };

    class Material
    {
    public:
        std::shared_ptr<Shader> m_shader;
        BufferLayout m_uniform_layout;
        std::vector<std::string> m_texture_names;

    protected:
        Material(MaterialInfo info)
            : m_shader(info.shader)
            , m_uniform_layout(info.uniform_layout)
            , m_texture_names(info.texture_names)
        {
        }

    public:
        Material(const Material& other) = delete;
        Material(Material&& other) = delete;
        Material& operator=(const Material& other) = delete;
        Material& operator=(Material&& other) = delete;
        virtual ~Material() {}
    };

    class MaterialInstance
    {
    protected:
        std::shared_ptr<Material> m_material;
        BufferData m_uniform_buffer_data;
        std::vector<std::shared_ptr<Texture>> m_texture_data;

    protected:
        MaterialInstance(std::shared_ptr<Material> material)
            : m_material(material)
            , m_uniform_buffer_data(material->m_uniform_layout)
            , m_texture_data(material->m_texture_names.size())
        {
        }

    public:
        virtual ~MaterialInstance() {}

        inline std::shared_ptr<Shader> GetShader()
        {
            return m_material->m_shader;
        }
        inline std::shared_ptr<Texture> GetTexture(int i)
        {
            return m_texture_data[i];
        }
        inline std::string GetTextureName(int i)
        {
            return m_material->m_texture_names[i];
        }
        inline int TextureCount() { return m_texture_data.size(); }
        inline const BufferLayout GetUniformLayout()
        {
            return m_material->m_uniform_layout;
        }
        inline const BufferData& GetUniformBufferData() const
        {
            return m_uniform_buffer_data;
        }
        inline BufferData& GetUniformBufferData()
        {
            return m_uniform_buffer_data;
        }

        void SetTexture(std::string name,
                        const std::shared_ptr<Texture>& texture);
    };
}  // namespace Merlin
#endif
