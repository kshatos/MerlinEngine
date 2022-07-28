#ifndef BUFFER_LAYOUT_HPP
#define BUFFER_LAYOUT_HPP
#include "glm/glm.hpp"
#include <string>
#include <vector>
#include <stdint.h>


namespace Merlin
{
    enum class ElementDataType
    {
        None = 0,
        Float,
        Float2,
        Float3,
        Float4,
        Mat3,
        Mat4,
        Int,
        Int2,
        Int3,
        Int4
    };

    uint32_t ElementDataTypeSize(const ElementDataType& type);

    uint32_t ElementDataTypeElementCount(const ElementDataType& type);

    struct BufferElement
    {
        std::string name;
        ElementDataType type;
        uint32_t offset;
        uint32_t size;
        bool normalized;

        BufferElement() :
            name(""),
            type(ElementDataType::None),
            offset(0),
            size(0),
            normalized(false)
        {
        }

        BufferElement(
            ElementDataType _type,
            const std::string _name,
            bool _normalized = false) :
            name(_name),
            type(_type),
            offset(0),
            size(ElementDataTypeSize(_type)),
            normalized(_normalized)
        {
        }
    };

    class BufferLayout
    {
        std::vector<BufferElement> m_elements;
        uint32_t m_stride;
    public:
        BufferLayout();
        BufferLayout(const std::initializer_list<BufferElement>& _elements);
        inline std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
        inline std::vector<BufferElement>::iterator end() { return m_elements.end(); }
        inline std::vector<BufferElement>::const_iterator begin() const { return m_elements.begin(); }
        inline std::vector<BufferElement>::const_iterator end() const { return m_elements.end(); }
        uint32_t GetStride() const;
    };

    class BufferData
    {
        BufferLayout m_layout;
        char* m_data;

    public:
        BufferData(BufferLayout layout);
        ~BufferData();
        void SetUniformFloat2(const std::string& name, const glm::vec2& data);
        void SetUniformFloat3(const std::string& name, const glm::vec3& data);
        void SetUniformFloat4(const std::string& name, const glm::vec4& data);
        void SetUniformMat3(const std::string& name, const glm::mat3& data);
        void SetUniformMat4(const std::string& name, const glm::mat4& data);
        void SetUniformInt(const std::string& name, const uint32_t& data);

        glm::vec2 GetUniformFloat2(const std::string& name);
        glm::vec3 GetUniformFloat3(const std::string& name);
        glm::vec4 GetUniformFloat4(const std::string& name);
        glm::mat3 GetUniformMat3(const std::string& name);
        glm::mat4 GetUniformMat4(const std::string& name);
        uint32_t GetUniformInt(const std::string& name);

    private:
        bool FindElement(
            const std::string& name,
            const ElementDataType& type,
            BufferElement& element);
    };

}
#endif