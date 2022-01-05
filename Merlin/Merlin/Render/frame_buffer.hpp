#ifndef FRAME_BUFFER_HPP
#define FRAME_BUFFER_HPP
#include <stdint.h>
#include <memory>


namespace Merlin
{
    enum class ColorBufferFormat
    {
        NONE,
        RGBA8
    };

    enum class DepthBufferFormat
    {
        NONE,
        DEPTH24_STENCIL8
    };


    struct FrameBufferParameters
    {
        uint32_t width;
        uint32_t height;
        ColorBufferFormat color_buffer_format;
        DepthBufferFormat depth_buffer_format;
    };

    class FrameBuffer
    {
    public:
        static std::shared_ptr<FrameBuffer> Create(const FrameBufferParameters& state);

        virtual ~FrameBuffer() {}

        virtual const FrameBufferParameters& GetParameters() const = 0;
        virtual FrameBufferParameters& GetParameters() = 0;
        virtual const uint32_t& GetColorAttachmentID() = 0;
        virtual const uint32_t& GetDepthAttachmentID() = 0;
        virtual void Bind() = 0;
        virtual void UnBind() = 0;
        virtual void Rebuild() = 0;
    };
}

#endif