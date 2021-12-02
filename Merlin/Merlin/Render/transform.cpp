#include "Merlin/Render/transform.hpp"
#include  <glm/gtx/transform.hpp>


namespace Merlin
{

    Transform::Transform() :
        position(glm::vec3(0.0f, 0.0f, 0.0f)),
        scale(glm::vec3(1.0f, 1.0f, 1.0f)),
        orientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)),
        transform_matrix_is_dirty(false),
        transform_matrix(glm::mat4(1.0f))
    {
    }

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
            auto tr = glm::translate(glm::mat4(1.0f), position);
            auto ro = glm::toMat4(orientation);
            auto sc = glm::scale(glm::mat4(1.0f), scale);
            transform_matrix = tr * ro * sc;

            transform_matrix_is_dirty = false;
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