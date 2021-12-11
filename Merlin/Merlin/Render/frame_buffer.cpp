#include "Merlin/Render/frame_buffer.hpp"
#include "Merlin/Render/opengl_frame_buffer.hpp"


namespace Merlin
{
    std::shared_ptr<FrameBuffer> FrameBuffer::Create(const FrameBufferParameters& state)
    {
        return std::make_shared<OpenGLFrameBuffer>(state);
    }
}
