#include "Merlin/Render/transform.hpp"

#include <glm/gtx/transform.hpp>

namespace Merlin
{

    Transform::Transform()
        : m_position(glm::vec3(0.0f, 0.0f, 0.0f))
        , m_scale(glm::vec3(1.0f, 1.0f, 1.0f))
        , m_orientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f))
        , m_transform_matrix_is_dirty(false)
        , m_transform_matrix(glm::mat4(1.0f))
    {
    }

    Transform::Transform(glm::vec3 position,
                         glm::vec3 scale,
                         glm::quat orientation)
        : m_position(position)
        , m_scale(scale)
        , m_orientation(orientation)
        , m_transform_matrix_is_dirty(true)
        , m_transform_matrix(glm::mat4())
    {
    }

    void Transform::SetPosition(glm::vec3 position) { m_position = position; }

    void Transform::SetScale(glm::vec3 scale) { m_scale = scale; }

    void Transform::SetOrientation(glm::vec3 orientation)
    {
        m_orientation = orientation;
    }

    void Transform::SetOrientation(glm::quat orientation)
    {
        m_orientation = orientation;
    }

    void Transform::SetEulerAnglesDegrees(glm::vec3 angles)
    {
        m_orientation = glm::quat(glm::radians(angles));
    }

    const glm::vec3& Transform::GetPosition() { return m_position; }

    const glm::vec3& Transform::GetScale() { return m_scale; }

    const glm::quat& Transform::GetOrientation() { return m_orientation; }

    const glm::vec3& Transform::GetEulerAnglesDegrees()
    {
        return glm::degrees(glm::eulerAngles(m_orientation));
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

    glm::vec3 Transform::Forward()
    {
        return m_orientation * glm::vec3(0.0, 0.0, -1.0);
    }

    glm::vec3 Transform::Up()
    {
        return m_orientation * glm::vec3(0.0, 1.0, 0.0);
    }

    glm::vec3 Transform::Right()
    {
        return m_orientation * glm::vec3(1.0, 0.0, 0.0);
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

    void Transform::Scale(float factor)
    {
        m_scale *= factor;
        m_transform_matrix_is_dirty = true;
    }

    void Transform::Rotate(glm::quat direction)
    {
        m_orientation *= direction;
        m_transform_matrix_is_dirty = true;
    }

    void Transform::Rotate(glm::vec3 axis, float angle_radians)
    {
        glm::quat rotation = glm::angleAxis(angle_radians, axis);
        m_orientation *= rotation;
        m_transform_matrix_is_dirty = true;
    }

    void Transform::RotateAround(glm::vec3 position,
                                 glm::vec3 axis,
                                 float angle_radians)
    {
        glm::quat rotation = glm::angleAxis(angle_radians, axis);

        auto relative_pos = m_position - position;
        relative_pos = rotation * relative_pos;

        m_position = position + relative_pos;
        m_orientation *= rotation;
        m_transform_matrix_is_dirty = true;
    }

    void Transform::LookAt(glm::vec3 target_position, glm::vec3 new_up)
    {
        auto direction = glm::normalize(target_position - m_position);
        m_orientation = glm::quatLookAt(direction, new_up);
        m_transform_matrix_is_dirty = true;
    }

}  // namespace Merlin