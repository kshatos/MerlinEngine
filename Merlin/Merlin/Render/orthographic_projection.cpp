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
    {
    }

    glm::mat4 OrthographicProjection::GetProjectionMatrix()
    {
        return m_projection_matrix;
    }

    void OrthographicProjection::SetAspectRatio(float aspect_ratio)
    {
        m_aspect_ratio = aspect_ratio;
        m_projection_matrix = glm::ortho(0.0f,
                                         m_vertical_size,
                                         0.0f,
                                         m_vertical_size * aspect_ratio,
                                         m_near_plane,
                                         m_far_plane);
    }

}  // namespace Merlin