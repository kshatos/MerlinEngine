#ifndef INDEX_BUFFER_HPP
#define INDEX_BUFFER_HPP
#include <stdint.h>

#include <memory>

namespace Merlin
{
    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer() {}
        virtual uint32_t GetCount() = 0;
    };
}  // namespace Merlin

#endif