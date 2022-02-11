#include "Merlin/Render/perspective_camera.hpp"
#include  <glm/gtx/transform.hpp>


namespace Merlin
{

    PerspectiveCamera::PerspectiveCamera(
        float fov,
        float aspect,
        float near,
        float far) :
        m_fov(fov),
        m_aspect(aspect),
        m_near(near),
        m_far(far),
        m_projection_matrix(glm::perspective(fov, aspect, near, far))
    {
    }

    glm::mat4 PerspectiveCamera::GetProjectionMatrix()
    {
        return m_projection_matrix;
    }

    void PerspectiveCamera::SetAspectRatio(float aspect_ratio)
    {
        m_aspect = aspect_ratio;
        m_projection_matrix = glm::perspective(m_fov, aspect_ratio, m_near, m_far);
    }

}