#ifndef MATERIAL_HPP
#define MATERIAL_HPP
#include <vector>
#include <string>
#include "Merlin/Render/buffer_layout.hpp"
#include "Merlin/Render/shader.hpp"
#include "Merlin/Render/texture2d.hpp"


namespace Merlin
{
    class Material
    {
    public:
        std::shared_ptr<Shader> m_shader;
        BufferLayout m_uniformLayout;
        std::vector<std::string> m_textureNames;
        char* m_uniformData;
        std::vector<std::shared_ptr<Texture>> m_textureData;

        Material(
            std::shared_ptr<Shader> shader,
            BufferLayout uniformLayout,
            std::vector<std::string> textureNames) :
            m_shader(shader),
            m_uniformLayout(uniformLayout),
            m_textureNames(textureNames),
            m_textureData(textureNames.size(), nullptr),
            m_uniformData(new char[uniformLayout.GetStride()])
        {
        }
        Material(const Material& other) = delete;
        Material(Material&& other) = delete;
        Material& operator=(const Material& other) = delete;
        Material& operator=(Material&& other) = delete;
        ~Material()
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
            const ShaderDataType& type,
            BufferElement& element);
    };
}
#endif
