#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include <stdint.h>
#include <memory>
#include <string>


namespace Merlin
{
    enum class TextureWrapMode
    {
        Repeat,
        MirroredRepeat,
        ClampToEdge,
        ClampToBorder,
    };

    enum class TextureFilterMode
    {
        Nearest,
        Linear,
    };

    struct Texture2DProperties
    {
        TextureWrapMode s_wrap_mode;
        TextureWrapMode t_wrap_mode;
        TextureFilterMode filter_mode;

        Texture2DProperties() :
            s_wrap_mode(TextureWrapMode::ClampToBorder),
            t_wrap_mode(TextureWrapMode::ClampToBorder),
            filter_mode(TextureFilterMode::Linear)
        
        {
        }

        Texture2DProperties(
            TextureWrapMode _s_wrap_mode,
            TextureWrapMode _t_wrap_mode,
            TextureFilterMode _filter_mode):
                s_wrap_mode(_s_wrap_mode),
                t_wrap_mode(_t_wrap_mode),
                filter_mode(_filter_mode)
        
        {
        }
    };

    class Texture2D
    {
    public:
        virtual ~Texture2D() {}
        virtual uint32_t GetWidth() = 0;
        virtual uint32_t GetHeight() = 0;
        virtual void Bind(uint32_t slot=0) = 0;
        virtual void UnBind(uint32_t slot=0) = 0;

        static std::shared_ptr<Texture2D> Create(
            const std::string& filepath,
            Texture2DProperties props=Texture2DProperties());

        static std::shared_ptr<Texture2D> Create(
            void* data,
            uint32_t width,
            uint32_t height,
            Texture2DProperties props = Texture2DProperties());
    };

}

#endif
