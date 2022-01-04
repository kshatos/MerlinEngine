#ifndef OPENGL_CUBEMAP_HPP
#define OPENGL_CUBEMAP_HPP
#include "Merlin/Render/cubemap.hpp"
#include <stdint.h>


namespace Merlin
{
    class OpenGLCubemap : public Cubemap
    {
        uint32_t m_channel_count;
        uint32_t m_resolution;
        uint32_t m_id;
    public:
        OpenGLCubemap(const std::vector<std::string>& face_paths);
        OpenGLCubemap(uint32_t resolution, uint32_t channel_count);
        ~OpenGLCubemap();

        inline virtual uint32_t GetResolution() override { return m_resolution; }
        inline virtual uint32_t GetChannelCount() override { return m_channel_count; }

        void Bind(uint32_t slot=0) override;
        void UnBind(uint32_t slot = 0) override;

        virtual void SetFaceData(CubeFace face, float* data) override;
    };
}
#endif