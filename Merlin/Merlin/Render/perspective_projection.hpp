#ifndef PERSPECTIVE_PROJECTION_HPP
#define PERSPECTIVE_PROJECTION_HPP
#include "Merlin/Render/camera_projection.hpp"

namespace Merlin
{
    class PerspectiveProjection : public CameraProjection
    {
        float m_fov;
        float m_aspect;
        float m_near;
        float m_far;
        glm::mat4 m_projection_matrix;
        bool m_is_dirty;

    public:
        PerspectiveProjection(float fov, float aspect, float near, float far);
        glm::mat4 GetProjectionMatrix() override;

        void SetAspectRatio(float aspect_ratio) override;
        void SetFOV(float fov);
        void SetNearPlane(float near_plane);
        void SetFarPlane(float far_plane);

        float GetAspectRatio();
        float GetFOV();
        float GetNearPlane();
        float GetFarPlane();
    };
}  // namespace Merlin
#endif