#ifndef BUFFER_LAYOUT_HPP
#define BUFFER_LAYOUT_HPP
#include <string>
#include <vector>
#include <stdint.h>


namespace Merlin
{
    enum class ShaderDataType
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

    uint32_t ShaderDataTypeSize(const ShaderDataType& type);

    uint32_t ShaderDataTypeElementCount(const ShaderDataType& type);

    struct BufferElement
    {
        std::string name;
        ShaderDataType type;
        uint32_t offset;
        uint32_t size;
        bool normalized;

        BufferElement(ShaderDataType _type, const std::string _name, bool _normalized=false) :
            name(_name),
            type(_type),
            offset(0),
            size(ShaderDataTypeSize(_type)),
            normalized(_normalized)
        {
        }
    };

    class BufferLayout
    {
        std::vector<BufferElement> elements;
        uint32_t stride;
    public:
        BufferLayout();
        BufferLayout(const std::initializer_list<BufferElement>& _elements);
        inline std::vector<BufferElement>::iterator begin() { return elements.begin(); }
        inline std::vector<BufferElement>::iterator end() { return elements.end(); }
        inline std::vector<BufferElement>::const_iterator begin() const { return elements.begin(); }
        inline std::vector<BufferElement>::const_iterator end() const { return elements.end(); }
        uint32_t GetStride() const;
    };

}
#endif