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

    struct DirectionalLightData
    {
        glm::vec3 direction;
        glm::vec3 color;
    };

    struct SpotLightData
    {
        glm::vec3 position;
        glm::vec3 direction;
        float cutoff;
        float intensity;
        float range;
        glm::vec3 color;
    };

}

#endif