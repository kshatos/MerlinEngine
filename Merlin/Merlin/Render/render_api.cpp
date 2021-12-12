#include "Merlin/Render/render_api.hpp"
#include "Merlin/Platform/OpenGL/opengl_render_api.hpp"


namespace Merlin
{
    RenderAPI* RenderAPI::Create()
    {
        return new OpenGLRenderAPI();
    }

}