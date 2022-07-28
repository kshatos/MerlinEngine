#include "Merlin/Render/buffer_data.hpp"
#include "Merlin/Core/logger.hpp"
#include <stdint.h>


namespace Merlin
{
    uint32_t ElementDataTypeSize(const ElementDataType& type)
    {
        switch (type)
        {
        case(ElementDataType::Float):  return 4 * 1;
        case(ElementDataType::Float2): return 4 * 2;
        case(ElementDataType::Float3): return 4 * 3;
        case(ElementDataType::Float4): return 4 * 4;
        case(ElementDataType::Mat3):   return 4 * 3 * 3;
        case(ElementDataType::Mat4):   return 4 * 4 * 4;
        case(ElementDataType::Int):    return 4 * 1;
        case(ElementDataType::Int2):   return 4 * 2;
        case(ElementDataType::Int3):   return 4 * 3;
        case(ElementDataType::Int4):   return 4 * 4;
        default: return 0;
        }
    }

    uint32_t ElementDataTypeElementCount(const ElementDataType& type)
    {
        switch (type)
        {
        case(ElementDataType::Float):  return 1;
        case(ElementDataType::Float2): return 2;
        case(ElementDataType::Float3): return 3;
        case(ElementDataType::Float4): return 4;
        case(ElementDataType::Mat3):   return 3 * 3;
        case(ElementDataType::Mat4):   return 4 * 4;
        case(ElementDataType::Int):    return 1;
        case(ElementDataType::Int2):   return 2;
        case(ElementDataType::Int3):   return 3;
        case(ElementDataType::Int4):   return 4;
        default: return 0;
        }
    }


    BufferLayout::BufferLayout() : m_stride(0) {}

    BufferLayout::BufferLayout(
        const std::initializer_list<BufferElement>& _elements) :
        m_elements(_elements)
    {
        uint32_t offset = 0;
        for (auto& element : m_elements)
        {
            element.offset = offset;
            offset += element.size;
        }
        m_stride = offset;
    }

    uint32_t BufferLayout::GetStride() const
    {
        return m_stride;
    }


    BufferData::BufferData(
        BufferLayout layout) :
        m_layout(layout),
        m_data(new char[layout.GetStride()])
    {
    }

    BufferData::~BufferData()
    {
        delete[] m_data;
    }

    void BufferData::SetUniformFloat2(
        const std::string& name,
        const glm::vec2& data)
    {
        BufferElement item;
        if (FindElement(name, ElementDataType::Float2, item))
        {
            glm::vec2* location = (glm::vec2*)(m_data + item.offset);
            *location = data;
        }
        else
        {
            ME_LOG_WARN("Unable to set uniform element: " + name);
        }
    }

    void BufferData::SetUniformFloat3(
        const std::string& name,
        const glm::vec3& data)
    {
        BufferElement item;
        if (FindElement(name, ElementDataType::Float3, item))
        {
            glm::vec3* location = (glm::vec3*)(m_data + item.offset);
            *location = data;
        }
        else
        {
            ME_LOG_WARN("Unable to set uniform element: " + name);
        }
    }

    void BufferData::SetUniformFloat4(
        const std::string& name,
        const glm::vec4& data)
    {
        BufferElement item;
        if (FindElement(name, ElementDataType::Float4, item))
        {
            glm::vec4* location = (glm::vec4*)(m_data + item.offset);
            *location = data;
        }
        else
        {
            ME_LOG_WARN("Unable to set uniform element: " + name);
        }
    }

    void BufferData::SetUniformMat3(
        const std::string& name,
        const glm::mat3& data)
    {
        BufferElement item;
        if (FindElement(name, ElementDataType::Mat3, item))
        {
            glm::mat3* location = (glm::mat3*)(m_data + item.offset);
            *location = data;
        }
        else
        {
            ME_LOG_WARN("Unable to set uniform element: " + name);
        }
    }

    void BufferData::SetUniformMat4(
        const std::string& name,
        const glm::mat4& data)
    {
        BufferElement item;
        if (FindElement(name, ElementDataType::Mat4, item))
        {
            glm::mat4* location = (glm::mat4*)(m_data + item.offset);
            *location = data;
        }
        else
        {
            ME_LOG_WARN("Unable to set uniform element: " + name);
        }
    }

    void BufferData::SetUniformInt(const std::string& name, const uint32_t& data)
    {
        BufferElement item;
        if (FindElement(name, ElementDataType::Int, item))
        {
            uint32_t* location = (glm::uint32_t*)(m_data + item.offset);
            *location = data;
        }
        else
        {
            ME_LOG_WARN("Unable to set uniform element: " + name);
        }
    }

    glm::vec2 BufferData::GetUniformFloat2(const std::string& name)
    {
        BufferElement item;
        if (FindElement(name, ElementDataType::Float2, item))
        {
            auto location = (glm::vec2*)(m_data + item.offset);
            return *location;
        }
        else
        {
            ME_LOG_WARN("Unable to get uniform element: " + name);
            return glm::vec2(0.0);
        }
    }

    glm::vec3 BufferData::GetUniformFloat3(const std::string& name)
    {
        BufferElement item;
        if (FindElement(name, ElementDataType::Float3, item))
        {
            auto location = (glm::vec3*)(m_data + item.offset);
            return *location;
        }
        else
        {
            ME_LOG_WARN("Unable to get uniform element: " + name);
            return glm::vec3(0.0);
        }
    }

    glm::vec4 BufferData::GetUniformFloat4(const std::string& name)
    {
        BufferElement item;
        if (FindElement(name, ElementDataType::Float4, item))
        {
            auto location = (glm::vec4*)(m_data + item.offset);
            return *location;
        }
        else
        {
            ME_LOG_WARN("Unable to get uniform element: " + name);
            return glm::vec4(0.0);
        }
    }

    glm::mat3 BufferData::GetUniformMat3(const std::string& name)
    {
        BufferElement item;
        if (FindElement(name, ElementDataType::Mat3, item))
        {
            auto location = (glm::mat3*)(m_data + item.offset);
            return *location;
        }
        else
        {
            ME_LOG_WARN("Unable to get uniform element: " + name);
            return glm::mat3(0.0);
        }
    }

    glm::mat4 BufferData::GetUniformMat4(const std::string& name)
    {
        BufferElement item;
        if (FindElement(name, ElementDataType::Mat4, item))
        {
            auto location = (glm::mat4*)(m_data + item.offset);
            return *location;
        }
        else
        {
            ME_LOG_WARN("Unable to get uniform element: " + name);
            return glm::mat4(0.0);
        }
    }

    uint32_t BufferData::GetUniformInt(const std::string& name)
    {
        BufferElement item;
        if (FindElement(name, ElementDataType::Int, item))
        {
            auto location = (uint32_t*)(m_data + item.offset);
            return *location;
        }
        else
        {
            ME_LOG_WARN("Unable to get uniform element: " + name);
            return 0;
        }
    }

    bool BufferData::FindElement(
        const std::string& name,
        const ElementDataType& type,
        BufferElement& element)
    {
        for (const auto& item : m_layout)
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

}
