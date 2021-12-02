#ifndef PERSPECTIVE_CAMERA_HPP
#define PERSPECTIVE_CAMERA_HPP
#include "Merlin/Render/camera.hpp"


namespace Merlin
{
    class PerspectiveCamera : public Camera
    {
        float fov;
        float aspect;
        float near;
        float far;

        glm::mat4 projection_matrix;
    public:
        PerspectiveCamera(float fov, float aspect, float near, float far);
        glm::mat4 GetProjectionMatrix() override;
    };
}
#endif