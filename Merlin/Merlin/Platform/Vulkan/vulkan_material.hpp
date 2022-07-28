#ifndef VULKAN_MATERIAL_HPP
#define VULKAN_MATERIAL_HPP
#include "Merlin/Render/material.hpp"


namespace Merlin
{
    class VulkanMaterial : public Material
    {
    public:
        VulkanMaterial(
            MaterialInfo info) :
            Material(info)
        {
        }
    };
}
#endif