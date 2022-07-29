#ifndef MATERIAL_HPP
#define MATERIAL_HPP
#include "Merlin/Render/buffer_data.hpp"
#include "Merlin/Render/shader.hpp"
#include "Merlin/Render/texture2d.hpp"
#include <vector>
#include <string>


namespace Merlin
{
    struct MaterialInfo
    {
        std::shared_ptr<Shader> shader;
        BufferLayout uniformLayout;
        std::vector<std::string> textureNames;

        MaterialInfo(
            std::shared_ptr<Shader> _shader,
            BufferLayout _uniformLayout,
            std::vector<std::string> _textureNames) :
            shader(_shader),
            uniformLayout(_uniformLayout),
            textureNames(_textureNames)
        {
        }
    };


    class Material
    {
    public:
        std::shared_ptr<Shader> m_shader;
        BufferLayout m_uniformLayout;
        std::vector<std::string> m_textureNames;

    protected:
        Material(MaterialInfo info) :
            m_shader(info.shader),
            m_uniformLayout(info.uniformLayout),
            m_textureNames(info.textureNames)
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
        BufferData m_uniformBufferData;
        std::vector<std::shared_ptr<Texture>> m_textureData;

    protected:
        MaterialInstance(std::shared_ptr<Material> material) :
            m_material(material),
            m_uniformBufferData(material->m_uniformLayout),
            m_textureData(material->m_textureNames.size())
        {
        }
    public:
        virtual ~MaterialInstance() {}

        inline std::shared_ptr<Shader> GetShader() { return m_material->m_shader; }
        inline std::shared_ptr<Texture> GetTexture(int i) { return m_textureData[i]; }
        inline std::string GetTextureName(int i) { return m_material->m_textureNames[i]; }
        inline int TextureCount() { return m_textureData.size(); }
        inline const BufferLayout GetUniformLayout() { return m_material->m_uniformLayout; }
        inline const BufferData& GetUniformBufferData() const { return m_uniformBufferData; }
        inline BufferData& GetUniformBufferData() { return m_uniformBufferData; }

        void SetTexture(std::string name, const std::shared_ptr<Texture>& texture);
    };
}
#endif
