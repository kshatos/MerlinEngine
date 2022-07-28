#ifndef OPENGL_MATERIAL_HPP
#define OPENGL_MATERIAL_HPP
#include "Merlin/Render/material.hpp"


namespace Merlin
{
    class OpenGLMaterial : public Material
    {
    public:
        OpenGLMaterial(
            MaterialInfo info) :
            Material(info)
        {
        }
    };
}
#endif