#ifndef FRAME_BUFFER_HPP
#define FRAME_BUFFER_HPP
#include <stdint.h>

#include <memory>

namespace Merlin
{
    enum class ColorBufferFormat
    {
        None,
        RGBA8
    };

    enum class DepthBufferFormat
    {
        None,
        Depth32,
        Depth24Stencil8
    };

    struct FrameBufferParameters
    {
        uint32_t width;
        uint32_t height;
        ColorBufferFormat color_buffer_format;
        DepthBufferFormat depth_buffer_format;

        FrameBufferParameters()
            : width(0)
            , height(0)
            , color_buffer_format(ColorBufferFormat::None)
            , depth_buffer_format(DepthBufferFormat::None)
        {
        }

        FrameBufferParameters(uint32_t _width,
                              uint32_t _height,
                              ColorBufferFormat _color_buffer_format,
                              DepthBufferFormat _depth_buffer_format)
            : width(_width)
            , height(_height)
            , color_buffer_format(_color_buffer_format)
            , depth_buffer_format(_depth_buffer_format)
        {
        }
    };

    class FrameBuffer
    {
    public:
        virtual ~FrameBuffer() {}

        virtual const FrameBufferParameters& GetParameters() const = 0;
        virtual FrameBufferParameters& GetParameters() = 0;
        virtual const uint32_t& GetColorAttachmentID() = 0;
        virtual const uint32_t& GetDepthAttachmentID() = 0;
        virtual void Rebuild() = 0;
    };
}  // namespace Merlin

#endif