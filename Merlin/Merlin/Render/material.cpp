#include "Merlin/Render/material.hpp"

namespace Merlin
{
    void MaterialInstance::SetTexture(std::string name,
                                      const std::shared_ptr<Texture>& texture)
    {
        int index = 0;
        for (const auto& texture_name : m_material->m_texture_names)
        {
            if (texture_name == name)
            {
                m_texture_data[index] = texture;
                return;
            }
            index++;
        }
    }
}  // namespace Merlin