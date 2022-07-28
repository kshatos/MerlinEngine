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
        char* m_uniformData;
        std::vector<std::shared_ptr<Texture>> m_textureData;

    protected:
        Material(MaterialInfo info) :
            m_shader(info.shader),
            m_uniformLayout(info.uniformLayout),
            m_textureNames(info.textureNames),
            m_uniformData(new char[info.uniformLayout.GetStride()]),
            m_textureData(info.textureNames.size())
        {
        }

    public:
        Material(const Material& other) = delete;
        Material(Material&& other) = delete;
        Material& operator=(const Material& other) = delete;
        Material& operator=(Material&& other) = delete;
        virtual ~Material()
        {
            delete[] m_uniformData;
        }

        inline int TextureCount() { return m_textureData.size(); }

        void SetUniformFloat(const std::string& name, const float& data);
        void SetUniformFloat2(const std::string& name, const glm::vec2& data);
        void SetUniformFloat3(const std::string& name, const glm::vec3& data);
        void SetUniformFloat4(const std::string& name, const glm::vec4& data);
        void SetUniformMat3(const std::string& name, const glm::mat3& data);
        void SetUniformMat4(const std::string& name, const glm::mat4& data);
        void SetUniformInt(const std::string& name, const uint32_t& data);

        void SetTexture(std::string name, const std::shared_ptr<Texture>& texture);

    private:
        bool FindBufferElement(
            const std::string& name,
            const ElementDataType& type,
            BufferElement& element);
    };
}
#endif
