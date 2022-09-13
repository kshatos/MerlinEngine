#ifndef CAMERA_PROJECTION_HPP
#define CAMERA_PROJECTION_HPP
#include <Merlin/Render/transform.hpp>
#include <glm/glm.hpp>

namespace Merlin
{
    class CameraProjection
    {
    public:
        virtual ~CameraProjection() {}
        virtual glm::mat4 GetProjectionMatrix() = 0;
        virtual void SetAspectRatio(float aspect_ratio) = 0;
    };
}  // namespace Merlin
#endif