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
        Transform(
            glm::vec3 position,
            glm::vec3 scale,
            glm::quat orientation);

        const glm::vec3& GetPosition();
        const glm::vec3& GetScale();
        const glm::quat& GetOrientation();
        const glm::mat4& GetTransformationMatrix();

        void Translate(glm::vec3 direction);
        void Scale(glm::vec3 direction);
        void Rotate(glm::quat direction);

    };
}
#endif