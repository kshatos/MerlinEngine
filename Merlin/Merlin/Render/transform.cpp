#include "Merlin/Render/transform.hpp"
#include  <glm/gtx/transform.hpp>


namespace Merlin
{

    Transform::Transform() :
        m_position(glm::vec3(0.0f, 0.0f, 0.0f)),
        m_scale(glm::vec3(1.0f, 1.0f, 1.0f)),
        m_orientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)),
        m_transform_matrix_is_dirty(false),
        m_transform_matrix(glm::mat4(1.0f))
    {
    }

    Transform::Transform(
        glm::vec3 position,
        glm::vec3 scale,
        glm::quat orientation) :
        m_position(position),
        m_scale(scale),
        m_orientation(orientation),
        m_transform_matrix_is_dirty(true),
        m_transform_matrix(glm::mat4())
    {
    }

    const glm::vec3& Transform::GetPosition()
    {
        return m_position;
    }

    const glm::vec3& Transform::GetScale()
    {
        return m_scale;
    }

    const glm::quat& Transform::GetOrientation()
    {
        return m_orientation;
    }

    const glm::mat4& Transform::GetTransformationMatrix()
    {
        if (m_transform_matrix_is_dirty)
        {
            auto tr = glm::translate(glm::mat4(1.0f), m_position);
            auto ro = glm::toMat4(m_orientation);
            auto sc = glm::scale(glm::mat4(1.0f), m_scale);
            m_transform_matrix = tr * ro * sc;

            m_transform_matrix_is_dirty = false;
        }
        return m_transform_matrix;
    }

    void Transform::Translate(glm::vec3 direction)
    {
        m_position += direction;
        m_transform_matrix_is_dirty = true;
    }

    void Transform::Scale(glm::vec3 direction)
    {
        m_scale *= direction;
        m_transform_matrix_is_dirty = true;
    }

    void Transform::Rotate(glm::quat direction)
    {
        m_orientation *= direction;
        m_transform_matrix_is_dirty = true;
    }

}