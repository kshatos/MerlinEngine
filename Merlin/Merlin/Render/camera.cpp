#include "Merlin/Render/camera.hpp"


namespace Merlin
{

    glm::mat4 Camera::GetViewMatrix()
    {
        return glm::inverse(m_transform.GetTransformationMatrix());
    }

    Transform& Camera::GetTransform()
    {
        return m_transform;
    }
}