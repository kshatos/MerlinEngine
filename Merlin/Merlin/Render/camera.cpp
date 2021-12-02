#include "Merlin/Render/camera.hpp"


namespace Merlin
{

    glm::mat4 Camera::GetViewMatrix()
    {
        return glm::inverse(transform.GetTransformationMatrix());
    }

    Transform& Camera::GetTransform()
    {
        return transform;
    }
}