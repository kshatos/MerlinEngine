#ifndef OPENGL_TEXTURE
#define OPENGL_TEXTURE
#include "Merlin/Render/texture2d.hpp"
#include "Merlin/Render/texture2d_data.hpp"
#include <string>


namespace Merlin
{
    class OpenGLTexture2D : public Texture2D
    {
        uint32_t m_id;

        void Init(
            const unsigned char* data,
            uint32_t width,
            uint32_t height,
            uint32_t channel_count,
            Texture2DProperties props);
    public:
        OpenGLTexture2D(
            Texture2DData texture_data,
            Texture2DProperties props);
        ~OpenGLTexture2D();
        void Bind(uint32_t slot);
        void UnBind(uint32_t slot);
    };
}

#endif
