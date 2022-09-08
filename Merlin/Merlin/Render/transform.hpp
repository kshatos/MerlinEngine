#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Merlin
{
    class Transform
    {
        glm::vec3 m_position;
        glm::vec3 m_scale;
        glm::quat m_orientation;

        bool m_transform_matrix_is_dirty;
        glm::mat4 m_transform_matrix;

    public:
        Transform();
        Transform(glm::vec3 position, glm::vec3 scale, glm::quat orientation);

        void SetPosition(glm::vec3 position);
        void SetScale(glm::vec3 scale);
        void SetOrientation(glm::vec3 orientation);
        void SetOrientation(glm::quat orientation);
        void SetEulerAnglesDegrees(glm::vec3 angles);

        const glm::vec3& GetPosition();
        const glm::vec3& GetScale();
        const glm::quat& GetOrientation();
        const glm::vec3& GetEulerAnglesDegrees();
        const glm::mat4& GetTransformationMatrix();

        glm::vec3 Forward();
        glm::vec3 Up();
        glm::vec3 Right();

        void Translate(glm::vec3 direction);
        void Scale(glm::vec3 direction);
        void Scale(float factor);
        void Rotate(glm::quat direction);
        void Rotate(glm::vec3 axis, float angle_radians);
        void RotateAround(glm::vec3 position,
                          glm::vec3 axis,
                          float angle_radians);
        void LookAt(glm::vec3 target_position,
                    glm::vec3 new_up = glm::vec3(0.0f, 1.0f, 0.0f));
    };
}  // namespace Merlin
#endif