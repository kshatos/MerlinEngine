#include "Merlin/Render/perspective_camera.hpp"
#include  <glm/gtx/transform.hpp>


namespace Merlin
{

    PerspectiveCamera::PerspectiveCamera(
        float fov,
        float aspect,
        float near,
        float far) :
            fov(fov),
            aspect(aspect),
            near(near),
            far(far),
            projection_matrix(glm::perspective(fov, aspect, near, far))
    {
    }

    glm::mat4 PerspectiveCamera::GetProjectionMatrix()
    {
        return projection_matrix;
    }
}