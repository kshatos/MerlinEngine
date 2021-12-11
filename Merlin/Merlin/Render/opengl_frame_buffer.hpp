#ifndef OPENGL_FRAME_BUFFER_HPP
#define OPENGL_FRAME_BUFFER_HPP
#include "Merlin/Render/frame_buffer.hpp"


namespace Merlin
{
    class OpenGLFrameBuffer : public FrameBuffer
    {
        FrameBufferParameters params;
        uint32_t id;
        uint32_t color_attachment_id;
        uint32_t depth_attachment_id;
    public:
        OpenGLFrameBuffer(FrameBufferParameters _params);
        ~OpenGLFrameBuffer();
        inline const FrameBufferParameters& GetParameters() const override { return params; }
        inline FrameBufferParameters& GetParameters() override { return params; }
        inline const uint32_t& GetColorAttachmentID() override { return color_attachment_id; }
        inline const uint32_t& GetDepthAttachmentID() override { return depth_attachment_id; }
        void Bind() override;
        void UnBind() override;
        void Rebuild() override;
    };
}

#endif