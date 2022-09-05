#include "merlin_editor_app.hpp"

#include "editor_gui_layer.hpp"

namespace MerlinEditor
{
    MerlinEditorApp::MerlinEditorApp(const Merlin::ApplicationInfo& properties)
        : Application(properties)
    {
        PushLayerBack(std::make_shared<EditorGUILayer>());
    }
}  // namespace MerlinEditor