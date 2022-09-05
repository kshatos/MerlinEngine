#include <imgui.h>

#include <Merlin/Core/core.hpp>

#include "editor_gui_layer.hpp"
#include "merlin_editor_app.hpp"

using namespace MerlinEditor;

void main()
{
    Merlin::ApplicationInfo app_info{
        "Merlin Editor",
        Merlin::RenderBackend::OpenGL,
        800,
        800,
    };
    MerlinEditorApp app(app_info);
    app.Run();
}