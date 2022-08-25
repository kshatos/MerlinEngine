#ifndef TEXTURE2D_HPP
#define TEXTURE2D_HPP
#include <stdint.h>

#include <memory>
#include <string>

#include "Merlin/Render/texture.hpp"

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

        Texture2DProperties()
            : s_wrap_mode(TextureWrapMode::ClampToBorder)
            , t_wrap_mode(TextureWrapMode::ClampToBorder)
            , filter_mode(TextureFilterMode::Linear)

        {
        }

        Texture2DProperties(TextureWrapMode _s_wrap_mode,
                            TextureWrapMode _t_wrap_mode,
                            TextureFilterMode _filter_mode)
            : s_wrap_mode(_s_wrap_mode)
            , t_wrap_mode(_t_wrap_mode)
            , filter_mode(_filter_mode)

        {
        }
    };

    class Texture2D : public Texture
    {
    protected:
        uint32_t m_width;
        uint32_t m_height;

    public:
        virtual ~Texture2D() {}
        inline uint32_t GetWidth() { return m_width; };
        inline uint32_t GetHeight() { return m_height; };
    };

}  // namespace Merlin

#endif
