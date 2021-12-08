#ifndef OPENGL_TEXTURE
#define OPENGL_TEXTURE
#include "Merlin/Render/texture2d.hpp"
#include <string>


namespace Merlin
{
    class OpenGLTexture2D : public Texture2D
    {
        uint32_t width;
        uint32_t height;
        uint32_t id;

        void Init(
            void* data,
            uint32_t _width,
            uint32_t _height,
            Texture2DProperties props);
    public:
        OpenGLTexture2D(
            const std::string& filepath,
            Texture2DProperties props);
        OpenGLTexture2D(
            void* data,
            uint32_t _width,
            uint32_t _height,
            Texture2DProperties props);
        ~OpenGLTexture2D();
        inline uint32_t GetWidth() override { return width; }
        inline uint32_t GetHeight() override { return height; }
        void Bind(uint32_t slot) override;
        void UnBind(uint32_t slot) override;
    };
}

#endif
