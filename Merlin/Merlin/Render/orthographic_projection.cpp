#include "Merlin/Render/orthographic_projection.hpp"

namespace Merlin
{
    OrthographicProjection::OrthographicProjection(float vertical_size,
                                                   float near_plane,
                                                   float far_plane,
                                                   float aspect_ratio)
        : m_vertical_size(vertical_size)
        , m_near_plane(near_plane)
        , m_far_plane(far_plane)
        , m_aspect_ratio(aspect_ratio)
        , m_projection_matrix(glm::ortho(0.0f,
                                         vertical_size,
                                         0.0f,
                                         vertical_size * aspect_ratio,
                                         m_near_plane,
                                         far_plane))
        , m_is_dirty(false)
    {
    }

    glm::mat4 OrthographicProjection::GetProjectionMatrix()
    {
        if (m_is_dirty)
        {
            m_projection_matrix = glm::ortho(0.0f,
                                             m_vertical_size,
                                             0.0f,
                                             m_vertical_size * m_aspect_ratio,
                                             m_near_plane,
                                             m_far_plane);
        }
        return m_projection_matrix;
    }

    void OrthographicProjection::SetAspectRatio(float aspect_ratio)
    {
        m_is_dirty = true;
        m_aspect_ratio = aspect_ratio;
    }

    void OrthographicProjection::SetVerticalSize(float vertical_size)
    {
        m_is_dirty = true;
        m_vertical_size = vertical_size;
    }
    void OrthographicProjection::SetNearPlane(float near_plane)
    {
        m_is_dirty = true;
        m_near_plane = near_plane;
    }
    void OrthographicProjection::SetFarPlane(float far_plane)
    {
        m_is_dirty = true;
        m_far_plane = far_plane;
    }

    float OrthographicProjection::GetAspectRatio() { return m_aspect_ratio; }
    float OrthographicProjection::GetVerticalSize() { return m_vertical_size; }
    float OrthographicProjection::GetNearPlane() { return m_near_plane; }
    float OrthographicProjection::GetFarPlane() { return m_far_plane; }

}  // namespace Merlin