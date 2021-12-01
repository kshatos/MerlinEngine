#ifndef INDEX_BUFFER_HPP
#define INDEX_BUFFER_HPP
#include <stdint.h>


namespace Merlin
{
    class IndexBuffer
    {
    public:
        static IndexBuffer* Create(uint32_t* indices, uint32_t count);

        virtual ~IndexBuffer() {}
        virtual void Bind() = 0;
        virtual void UnBind() = 0;
        virtual uint32_t GetCount() = 0;
    };
}

#endif