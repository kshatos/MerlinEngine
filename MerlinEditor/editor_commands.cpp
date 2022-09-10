#include "editor_commands.hpp"

#include <Merlin/Scene/scene_serialization.hpp>

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
        m_serialized_entity =
            Merlin::SceneSerializer::SerializeEntity(entity.value());
        entity->Destroy();
    }

    void DestroyEntityCommand::Undo()
    {
        auto entity = m_scene->CreateEntity(m_entity_uuid);
        Merlin::SceneSerializer::DeserializeEntity(
            m_serialized_entity, entity, m_scene);
    }

    void DestroyEntityCommand::Redo()
    {
        auto entity = m_scene->GetEntity(m_entity_uuid);
        entity->Destroy();
    }

    void EntityReplaceParentCommand::Do()
    {
        auto parent_entity = m_scene->GetEntity(m_parent_uuid);
        auto child_entity = m_scene->GetEntity(m_child_uuid);

        m_parent_state =
            Merlin::SceneSerializer::SerializeEntity(parent_entity.value());
        m_child_state =
            Merlin::SceneSerializer::SerializeEntity(child_entity.value());

        child_entity->RemoveParent();
        parent_entity->AddChild(child_entity.value());
    }

    void EntityReplaceParentCommand::Undo()
    {
        auto parent_entity = m_scene->GetEntity(m_parent_uuid);
        auto child_entity = m_scene->GetEntity(m_child_uuid);

        Merlin::SceneSerializer::DeserializeEntity(
            m_parent_state, parent_entity.value(), m_scene);
        Merlin::SceneSerializer::DeserializeEntity(
            m_child_state, child_entity.value(), m_scene);
    }

    void EntityReplaceParentCommand::Redo()
    {
        auto parent_entity = m_scene->GetEntity(m_parent_uuid);
        auto child_entity = m_scene->GetEntity(m_child_uuid);

        child_entity->RemoveParent();
        parent_entity->AddChild(child_entity.value());
    }

}  // namespace MerlinEditor
