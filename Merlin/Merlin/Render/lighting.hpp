#ifndef LIGHTING_HPP
#define LIGHTING_HPP
#include <glm/glm.hpp>


namespace Merlin
{
    struct PointLightData
    {
        glm::vec3 position;
        float radiantFlux;
        float range;
        glm::vec3 color;
    };

    struct DirectionalLightData
    {
        glm::vec3 direction;
        float irradiance;
        glm::vec3 color;
    };

    struct SpotLightData
    {
        glm::vec3 position;
        glm::vec3 direction;
        float cutoffAngle;
        float falloffRatio;
        float radiantIntensity;
        float range;
        glm::vec3 color;
    };

}

#endif