#ifndef ORTHOGRAPHIC_PROJECTION_HPP
#define ORTHOGRAPHIC_PROJECTION_HPP
#include "Merlin/Render/camera_projection.hpp"

namespace Merlin
{
    class OrthographicProjection : public CameraProjection
    {
        float m_vertical_size;
        float m_near_plane;
        float m_far_plane;
        float m_aspect_ratio;
        glm::mat4 m_projection_matrix;

    public:
        OrthographicProjection(float vertical_size,
                           float near_plane,
                           float far_plane,
                           float aspect_ratio);
        virtual glm::mat4 GetProjectionMatrix() override;
        virtual void SetAspectRatio(float aspect_ratio) override;
    };
}  // namespace Merlin
#endif