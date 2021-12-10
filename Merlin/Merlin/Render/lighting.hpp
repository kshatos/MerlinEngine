#ifndef LIGHTING_HPP
#define LIGHTING_HPP
#include <glm/glm.hpp>


namespace Merlin
{
    struct PointLightData
    {
        glm::vec3 position;
        float intensity;
        float range;
        glm::vec3 color;
    };
}

#endif