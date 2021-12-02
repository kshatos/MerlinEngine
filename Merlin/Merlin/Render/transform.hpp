#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>


namespace Merlin
{
    class Transform
    {
        glm::vec3 position;
        glm::vec3 scale;
        glm::quat orientation;

        bool transform_matrix_is_dirty;
        glm::mat4 transform_matrix;
    public:
        Transform();
        Transform(
            glm::vec3 _position,
            glm::vec3 _scale,
            glm::quat _orientation);

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