#ifndef OPENGL_FRAME_BUFFER_HPP
#define OPENGL_FRAME_BUFFER_HPP
#include "Merlin/Render/frame_buffer.hpp"

namespace Merlin
{
    class OpenGLFrameBuffer : public FrameBuffer
    {
        FrameBufferParameters m_parameters;
        uint32_t m_id;
        uint32_t m_color_attachment_id;
        uint32_t m_depth_attachment_id;

    public:
        OpenGLFrameBuffer(FrameBufferParameters parameters);
        ~OpenGLFrameBuffer();
        inline const FrameBufferParameters& GetParameters() const override
        {
            return m_parameters;
        }
        inline FrameBufferParameters& GetParameters() override
        {
            return m_parameters;
        }
        inline const uint32_t& GetColorAttachmentID() override
        {
            return m_color_attachment_id;
        }
        inline const uint32_t& GetDepthAttachmentID() override
        {
            return m_depth_attachment_id;
        }
        void Bind();
        void UnBind();
        void BindColorTexture(uint32_t slot);
        void BindDepthTexture(uint32_t slot);
        void UnBindColorTexture(uint32_t slot);
        void UnBindDepthTexture(uint32_t slot);
        void Rebuild() override;
    };
}  // namespace Merlin

#endif