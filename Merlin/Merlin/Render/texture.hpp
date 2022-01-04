#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include <stdint.h>


namespace Merlin
{
    class Texture
    {
    public:
        virtual ~Texture() {}
        virtual void Bind(uint32_t slot) = 0;
        virtual void UnBind(uint32_t slot) = 0;
    };
}
#endif
