#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include <stdint.h>
#include <memory>


namespace Merlin
{
    class Texture2D
    {
    public:
        virtual ~Texture2D() {}
        virtual uint32_t GetWidth() = 0;
        virtual uint32_t GetHeight() = 0;
        virtual void Bind(uint32_t slot=0) = 0;
        virtual void UnBind(uint32_t slot=0) = 0;

        static std::shared_ptr<Texture2D> Create(void* data, size_t size,  uint32_t width, uint32_t height);
    };
}

#endif
