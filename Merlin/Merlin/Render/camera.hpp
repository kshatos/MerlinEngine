#ifndef CAMERA_HPP
#define CAMERA_HPP
#include <Merlin/Render/transform.hpp>
#include <glm/glm.hpp>

namespace Merlin
{
    class Camera
    {
    public:
        virtual ~Camera() {}
        virtual glm::mat4 GetProjectionMatrix() = 0;
        virtual void SetAspectRatio(float aspect_ratio) = 0;
    };
}  // namespace Merlin
#endif