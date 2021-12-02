#ifndef CAMERA_HPP
#define CAMERA_HPP
#include <glm/glm.hpp>


namespace Merlin
{
    class Camera
    {
    public:
        virtual ~Camera() {}
        virtual glm::mat4 GetProjectionMatrix() = 0;
    };
}
#endif