#include "editor_commands.hpp"

namespace MerlinEditor
{
    void EditorCommandQueue::DoCommand(std::shared_ptr<EditorCommand> command)
    {
        command->Do();
        m_undo_queue.push(command);

        while (!m_redo_queue.empty()) m_redo_queue.pop();
    }

    void EditorCommandQueue::UndoCommand()
    {
        auto command = m_undo_queue.front();
        m_undo_queue.pop();

        command->Undo();
        m_redo_queue.push(command);
    }

    void EditorCommandQueue::RedoCommand()
    {
        if (m_redo_queue.empty()) return;

        auto command = m_redo_queue.front();
        m_redo_queue.pop();

        command->Redo();
        m_undo_queue.push(command);
    }

}  // namespace MerlinEditor
