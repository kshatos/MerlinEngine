#include "Merlin/Render/material.hpp"


namespace Merlin
{

    bool Material::FindBufferElement(
        const std::string& name,
        const ShaderDataType& type,
        BufferElement& element)
    {
        for (const auto& item : m_uniformLayout)
        {
            if (item.name != name)
                continue;
            if (item.type != type)
                continue;
            element = item;
            return true;
        }
        return false;
    }

    void Material::SetUniformFloat(
        const std::string& name,
        const float& data)
    {
        BufferElement item;
        if (FindBufferElement(name, ShaderDataType::Float, item))
        {
            float* location = (float*)(m_uniformData + item.offset);
            *location = data;
        }
        else
        {
            m_shader->SetUniformFloat(name, data);
        }
    }

    void Material::SetUniformFloat2(const std::string& name, const glm::vec2& data)
    {
        BufferElement item;
        if (FindBufferElement(name, ShaderDataType::Float2, item))
        {
            glm::vec2* location = (glm::vec2*)(m_uniformData + item.offset);
            *location = data;
        }
        else
        {
            m_shader->SetUniformFloat2(name, data);
        }
    }

    void Material::SetUniformFloat3(const std::string& name, const glm::vec3& data)
    {
        BufferElement item;
        if (FindBufferElement(name, ShaderDataType::Float3, item))
        {
            glm::vec3* location = (glm::vec3*)(m_uniformData + item.offset);
            *location = data;
        }
        else
        {
            m_shader->SetUniformFloat3(name, data);
        }
    }

    void Material::SetUniformFloat4(const std::string& name, const glm::vec4& data)
    {
        BufferElement item;
        if (FindBufferElement(name, ShaderDataType::Float4, item))
        {
            glm::vec4* location = (glm::vec4*)(m_uniformData + item.offset);
            *location = data;
        }
        else
        {
            m_shader->SetUniformFloat4(name, data);
        }
    }

    void Material::SetUniformMat3(const std::string& name, const glm::mat3& data)
    {
        BufferElement item;
        if (FindBufferElement(name, ShaderDataType::Mat3, item))
        {
            glm::mat3* location = (glm::mat3*)(m_uniformData + item.offset);
            *location = data;
        }
        else
        {
            m_shader->SetUniformMat3(name, data);
        }
    }

    void Material::SetUniformMat4(const std::string& name, const glm::mat4& data)
    {
        BufferElement item;
        if (FindBufferElement(name, ShaderDataType::Mat4, item))
        {
            glm::mat4* location = (glm::mat4*)(m_uniformData + item.offset);
            *location = data;
        }
        else
        {
            m_shader->SetUniformMat4(name, data);
        }
    }

    void Material::SetUniformInt(const std::string& name, const uint32_t& data)
    {
        BufferElement item;
        if (FindBufferElement(name, ShaderDataType::Int, item))
        {
            uint32_t* location = (uint32_t*)(m_uniformData + item.offset);
            *location = data;
        }
        else
        {
            m_shader->SetUniformInt(name, data);
        }
    }

    void Material::SetTexture(std::string name, const std::shared_ptr<Texture2D> texture)
    {
        for (int i = 0; i < m_textureNames.size(); ++i)
        {
            if (m_textureNames[i] != name)
                continue;
            m_textureData[i] = texture;;
        }
    }

    void Material::Bind()
    {
        m_shader->Bind();
        for (const auto& item : m_uniformLayout)
        {
            m_shader->SetUniform(item.name, item.type, m_uniformData + item.offset);
        }
        for (int i = 0; i < m_textureData.size(); ++i)
        {
            if (m_textureData[i])
            {
                m_shader->SetUniformInt(m_textureNames[i], i);
                m_textureData[i]->Bind(i);
            }
        }
    }

    void Material::UnBind()
    {
        m_shader->UnBind();
        for (int i = 0; i < m_textureData.size(); ++i)
        {
            if (m_textureData[i])
                m_textureData[i]->UnBind(i);
        }
    }
}