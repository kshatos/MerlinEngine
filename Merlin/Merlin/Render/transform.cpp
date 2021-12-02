#include "Merlin/Render/transform.hpp"


namespace Merlin
{
    Transform::Transform(
        glm::vec3 _position,
        glm::vec3 _scale,
        glm::quat _orientation) :
        position(_position),
        scale(_scale),
        orientation(_orientation),
        transform_matrix_is_dirty(true),
        transform_matrix(glm::mat4())
    {
    }

    const glm::vec3& Transform::GetPosition()
    {
        return position;
    }

    const glm::vec3& Transform::GetScale()
    {
        return scale;
    }

    const glm::quat& Transform::GetOrientation()
    {
        return orientation;
    }

    const glm::mat4& Transform::GetTransformationMatrix()
    {
        if (transform_matrix_is_dirty)
        {
            transform_matrix = (
                glm::scale(glm::mat4(), scale) *
                glm::translate(glm::mat4(), position) *
                glm::toMat4(orientation));
        }
        return transform_matrix;
    }

    void Transform::Translate(glm::vec3 direction)
    {
        position += direction;
        transform_matrix_is_dirty = true;
    }

    void Transform::Scale(glm::vec3 direction)
    {
        scale *= direction;
        transform_matrix_is_dirty = true;
    }

    void Transform::Rotate(glm::quat direction)
    {
        orientation *= direction;
        transform_matrix_is_dirty = true;
    }

}