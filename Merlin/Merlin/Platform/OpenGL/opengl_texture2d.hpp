#ifndef OPENGL_TEXTURE
#define OPENGL_TEXTURE
#include "Merlin/Render/texture2d.hpp"
#include <string>


namespace Merlin
{
    class OpenGLTexture2D : public Texture2D
    {
        uint32_t m_width;
        uint32_t m_height;
        uint32_t m_id;

        void Init(
            void* data,
            uint32_t width,
            uint32_t height,
            uint32_t channel_count,
            Texture2DProperties props);
    public:
        OpenGLTexture2D(
            const std::string& filepath,
            Texture2DProperties props);
        OpenGLTexture2D(
            void* data,
            uint32_t width,
            uint32_t height,
            uint32_t channel_count,
            Texture2DProperties props);
        ~OpenGLTexture2D();
        inline uint32_t GetWidth() override { return m_width; }
        inline uint32_t GetHeight() override { return m_height; }
        void Bind(uint32_t slot);
        void UnBind(uint32_t slot);
    };
}

#endif
