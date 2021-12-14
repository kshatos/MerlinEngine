#ifndef OPENGL_CUBEMAP_HPP
#define OPENGL_CUBEMAP_HPP
#include "Merlin/Render/cubemap.hpp"
#include <stdint.h>


namespace Merlin
{
    class OpenGLCubemap : public Cubemap
    {
        uint32_t m_id;
    public:
        OpenGLCubemap(const std::vector<std::string>& face_paths);
        ~OpenGLCubemap();
        void Bind() override;
        void UnBind() override;
    };
}
#endif