#ifndef VULKAN_MATERIAL_HPP
#define VULKAN_MATERIAL_HPP
#include "Merlin/Render/material.hpp"

namespace Merlin
{
    class VulkanMaterial : public Material
    {
    public:
        VulkanMaterial(MaterialInfo info) : Material(info) {}
    };

    class VulkanMaterialInstance : public MaterialInstance
    {
    public:
        VulkanMaterialInstance(std::shared_ptr<Material> material)
            : MaterialInstance(material)
        {
        }
    };
}  // namespace Merlin
#endif