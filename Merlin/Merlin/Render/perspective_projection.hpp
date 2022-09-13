#ifndef PERSPECTIVE_PROJECTION_HPP
#define PERSPECTIVE_PROJECTION_HPP
#include "Merlin/Render/camera_projection.hpp"

namespace Merlin
{
    class PerspectiveCamera : public CameraProjection
    {
        float m_fov;
        float m_aspect;
        float m_near;
        float m_far;
        glm::mat4 m_projection_matrix;

    public:
        PerspectiveCamera(float fov, float aspect, float near, float far);
        glm::mat4 GetProjectionMatrix() override;
        void SetAspectRatio(float aspect_ratio) override;
    };
}  // namespace Merlin
#endif