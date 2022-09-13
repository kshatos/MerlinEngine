#include "Merlin/Render/perspective_projection.hpp"

#include <glm/gtx/transform.hpp>

namespace Merlin
{

    PerspectiveProjection::PerspectiveProjection(float fov,
                                                 float aspect,
                                                 float near,
                                                 float far)
        : m_fov(fov)
        , m_aspect(aspect)
        , m_near(near)
        , m_far(far)
        , m_projection_matrix(glm::perspective(fov, aspect, near, far))
        , m_is_dirty(false)
    {
    }

    glm::mat4 PerspectiveProjection::GetProjectionMatrix()
    {
        if (m_is_dirty)
        {
            m_projection_matrix =
                glm::perspective(m_fov, m_aspect, m_near, m_far);
            m_is_dirty = false;
        }
        return m_projection_matrix;
    }

    void PerspectiveProjection::SetAspectRatio(float aspect_ratio)
    {
        m_aspect = aspect_ratio;
        m_is_dirty = true;
    }

    void PerspectiveProjection::SetFOV(float fov)
    {
        m_fov = fov;
        m_is_dirty = true;
    }

    void PerspectiveProjection::SetNearPlane(float near_plane)
    {
        m_near = near_plane;
        m_is_dirty = true;
    }
    void PerspectiveProjection::SetFarPlane(float far_plane)
    {
        m_far = far_plane;
        m_is_dirty = true;
    }

    float PerspectiveProjection::GetAspectRatio() { return m_aspect; }
    float PerspectiveProjection::GetFOV() { return m_fov; }
    float PerspectiveProjection::GetNearPlane() { return m_near; }
    float PerspectiveProjection::GetFarPlane() { return m_far; }

}  // namespace Merlin