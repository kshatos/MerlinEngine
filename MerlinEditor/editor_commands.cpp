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
            m_undo_stack.push(command);
        }

        while (!m_redo_stack.empty()) m_redo_stack.pop();
    }

    void EditorCommandQueue::UndoCommand()
    {
        if (m_undo_stack.empty()) return;

        auto command = m_undo_stack.top();
        m_undo_stack.pop();

        command->Undo();
        m_redo_stack.push(command);
    }

    void EditorCommandQueue::RedoCommand()
    {
        if (m_redo_stack.empty()) return;

        auto command = m_redo_stack.top();
        m_redo_stack.pop();

        command->Redo();
        m_undo_stack.push(command);
    }

    void EditorCommandQueue::Clear()
    {
        while (!m_do_queue.empty()) m_do_queue.pop();
        while (!m_undo_stack.empty()) m_undo_stack.pop();
        while (!m_redo_stack.empty()) m_redo_stack.pop();
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

    void DestroyEntityCommand::Do()
    {
        auto entity = m_scene->GetEntity(m_entity_uuid);
        entity->Destroy();
    }

    void DestroyEntityCommand::Undo() { m_scene->CreateEntity(m_entity_uuid); }

    void DestroyEntityCommand::Redo()
    {
        auto entity = m_scene->GetEntity(m_entity_uuid);
        entity->Destroy();
    }

}  // namespace MerlinEditor
