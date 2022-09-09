#ifndef EDITOR_PANEL_HPP
#define EDITOR_PANEL_HPP
#include <functional>

#include "editor_commands.hpp"

namespace MerlinEditor
{

    class EditorPanel
    {
        std::function<void(std::shared_ptr<EditorCommand>)> m_command_callback;

    public:
        virtual ~EditorPanel() {}
        inline void SetCommandCallback(
            std::function<void(std::shared_ptr<EditorCommand>)> callback)
        {
            m_command_callback = callback;
        }

    protected:
        inline void SubmitCommand(std::shared_ptr<EditorCommand> command)
        {
            m_command_callback(command);
        }
    };
}  // namespace MerlinEditor
#endif