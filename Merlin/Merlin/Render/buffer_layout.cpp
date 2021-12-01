#include "Merlin/Render/buffer_layout.hpp"

namespace Merlin
{
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

    uint32_t ShaderDataTypeElementCount(const ShaderDataType& type)
    {
        switch (type)
        {
        case(ShaderDataType::Float):  return 1;
        case(ShaderDataType::Float2): return 2;
        case(ShaderDataType::Float3): return 3;
        case(ShaderDataType::Float4): return 4;
        case(ShaderDataType::Mat3):   return 3 * 3;
        case(ShaderDataType::Mat4):   return 4 * 4;
        case(ShaderDataType::Int):    return 1;
        case(ShaderDataType::Int2):   return 2;
        case(ShaderDataType::Int3):   return 3;
        case(ShaderDataType::Int4):   return 4;
        default: return 0;
        }
    }

    BufferLayout::BufferLayout() : stride(0) {}

    BufferLayout::BufferLayout(
        const std::initializer_list<BufferElement>& _elements) :
        elements(_elements)
    {
        uint32_t offset = 0;
        for (auto& element : elements)
        {
            element.offset = offset;
            offset += element.size;
        }
        stride = offset;
    }

    uint32_t BufferLayout::GetStride()
    {
        return stride;
    }

}
