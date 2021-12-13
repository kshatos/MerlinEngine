#include "Merlin/Render/vertex_array.hpp"
#include "Merlin/Platform/OpenGL/opengl_vertex_array.hpp"


namespace Merlin
{
    std::shared_ptr<VertexArray> VertexArray::Create()
    {
        return std::make_shared<OpenGLVertexArray>();
    }
}