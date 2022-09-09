#include "editor_commands.hpp"

namespace MerlinEditor
{
    void EditorCommandQueue::AddCommand(std::shared_ptr<EditorCommand> command)
    {
        m_do_queue.push(command);
    }

    void EditorCommandQueue::DoCommands()
    {
        if (m_do_queue.empty()) return;

        while (!m_do_queue.empty())
        {
            auto command = m_do_queue.front();
            m_do_queue.pop();

            command->Do();
            m_undo_queue.push(command);
        }

        while (!m_redo_queue.empty()) m_redo_queue.pop();
    }

    void EditorCommandQueue::UndoCommand()
    {
        if (m_undo_queue.empty()) return;

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

    void EditorCommandQueue::Clear()
    {
        while (!m_do_queue.empty()) m_do_queue.pop();
        while (!m_undo_queue.empty()) m_undo_queue.pop();
        while (!m_redo_queue.empty()) m_redo_queue.pop();
    }

    void CreateEntityCommand::Do()
    {
        auto entity = m_scene->CreateEntity();
        m_entity_uuid = entity.GetUUID();
    }

    void CreateEntityCommand::Undo()
    {
        auto entity = m_scene->GetEntity(m_entity_uuid);
        entity->Destroy();
    }

    void CreateEntityCommand::Redo() { m_scene->CreateEntity(m_entity_uuid); }

}  // namespace MerlinEditor
