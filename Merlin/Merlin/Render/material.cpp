#include "Merlin/Render/material.hpp"


namespace Merlin
{
    void MaterialInstance::SetTexture(
        std::string name,
        const std::shared_ptr<Texture>& texture)
    {
        int index = 0;
        for (const auto& texture_name : m_material->m_textureNames)
        {
            if (texture_name == name)
            {
                m_textureData[index] = texture;
                return;
            }
            index++;
        }
    }
}