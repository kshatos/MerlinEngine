#ifndef CAMERA_HPP
#define CAMERA_HPP
#include <glm/glm.hpp>
#include <Merlin/Render/transform.hpp>


namespace Merlin
{
    class Camera
    {
        Transform m_transform;
    public:
        virtual ~Camera() {}
        virtual glm::mat4 GetProjectionMatrix() = 0;
        glm::mat4 GetViewMatrix();
        Transform& GetTransform();
    };
}
#endif