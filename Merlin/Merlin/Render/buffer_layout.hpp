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

    uint32_t ShaderDataTypeSize(const ShaderDataType& type)
    {
        switch (type)
        {
        case(ShaderDataType::Float):  return 4 * 1;
        case(ShaderDataType::Float2): return 4 * 2;
        case(ShaderDataType::Float3): return 4 * 3;
        case(ShaderDataType::Float4): return 4 * 4;
        case(ShaderDataType::Mat3):   return 4 * 3 * 3;
        case(ShaderDataType::Mat4):   return 4 * 4 * 4;
        case(ShaderDataType::Int):    return 4 * 1;
        case(ShaderDataType::Int2):   return 4 * 2;
        case(ShaderDataType::Int3):   return 4 * 3;
        case(ShaderDataType::Int4):   return 4 * 4;
        default: return 0;
        }
    }

    struct BufferElement
    {
        std::string name;
        ShaderDataType type;
        uint32_t offset;
        uint32_t size;

        BufferElement(ShaderDataType _type, const std::string _name) :
            name(_name),
            type(_type),
            size(ShaderDataTypeSize(_type)),
            offset(0)
        {
        }
    };

    class BufferLayout
    {
        std::vector<BufferElement> elements;
        uint32_t stride;
    public:
        BufferLayout(const std::initializer_list<BufferElement>& _elements) : elements(_elements)
        {
            uint32_t offset = 0;
            for (auto& element : elements)
            {
                element.offset = offset;
                offset += element.size;
            }
            stride = offset;
        }
    };

}

#endif