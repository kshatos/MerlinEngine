#include "Merlin/Render/vertex_array.hpp"
#include "Merlin/Render/opengl_vertex_array.hpp"


namespace Merlin
{
    VertexArray* VertexArray::Create()
    {
        return new OpenGLVertexArray();
    }
}