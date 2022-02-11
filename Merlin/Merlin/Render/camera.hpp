#ifndef CAMERA_HPP
#define CAMERA_HPP
#include <glm/glm.hpp>
#include <Merlin/Render/transform.hpp>


namespace Merlin
{
    class Camera
    {
    public:
        virtual ~Camera() {}
        virtual glm::mat4 GetProjectionMatrix() = 0;
        virtual void SetAspectRatio(float aspect_ratio) = 0;
    };
}
#endif