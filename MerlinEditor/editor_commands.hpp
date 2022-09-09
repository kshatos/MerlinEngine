#ifndef EDITOR_COMMANDS_HPP
#define EDITOR_COMMANDS_HPP
#include <Merlin/Scene/game_scene.hpp>
#include <memory>
#include <queue>
#include <stack>

namespace MerlinEditor
{
    class EditorCommand
    {
    public:
        virtual ~EditorCommand() {}
        virtual void Do() = 0;
        virtual void Undo() = 0;
        virtual void Redo() = 0;
    };

    class EditorCommandQueue
    {
        std::queue<std::shared_ptr<EditorCommand>> m_do_queue;
        std::stack<std::shared_ptr<EditorCommand>> m_undo_stack;
        std::stack<std::shared_ptr<EditorCommand>> m_redo_stack;

    public:
        void AddCommand(std::shared_ptr<EditorCommand> command);
        void DoCommands();
        void UndoCommand();
        void RedoCommand();
        void Clear();
    };

    // TODO: Use serialization to restore entity components on recreate
    class CreateEntityCommand : public EditorCommand
    {
        std::shared_ptr<Merlin::GameScene> m_scene;
        Merlin::UUID m_entity_uuid;

    public:
        CreateEntityCommand(std::shared_ptr<Merlin::GameScene> scene)
            : m_scene(scene), m_entity_uuid(Merlin::UUID::Nil())
        {
        }

        virtual void Do() override;
        virtual void Undo() override;
        virtual void Redo() override;
    };

    class DestroyEntityCommand : public EditorCommand
    {
        std::shared_ptr<Merlin::GameScene> m_scene;
        Merlin::UUID m_entity_uuid;

    public:
        DestroyEntityCommand(std::shared_ptr<Merlin::GameScene> scene,
                             Merlin::UUID uuid)
            : m_scene(scene), m_entity_uuid(uuid)
        {
        }
        virtual void Do() override;
        virtual void Undo() override;
        virtual void Redo() override;
    };

    template <typename T>
    class AddComponentCommand : public EditorCommand
    {
        std::shared_ptr<Merlin::GameScene> m_scene;
        Merlin::UUID m_entity_uuid;

    public:
        AddComponentCommand(std::shared_ptr<Merlin::GameScene> scene,
                            Merlin::UUID uuid)
            : m_scene(scene), m_entity_uuid(uuid)
        {
        }

        virtual void Do() override
        {
            auto entity = m_scene->GetEntity(m_entity_uuid);
            entity->AddComponent<T>();
        }

        virtual void Undo() override
        {
            auto entity = m_scene->GetEntity(m_entity_uuid);
            entity->RemoveComponent<T>();
        }

        virtual void Redo() override
        {
            auto entity = m_scene->GetEntity(m_entity_uuid);
            entity->AddComponent<T>();
        }
    };

    template <typename T>
    class RemoveComponentCommand : public EditorCommand
    {
        std::shared_ptr<Merlin::GameScene> m_scene;
        Merlin::UUID m_entity_uuid;
        T m_component_data;

    public:
        RemoveComponentCommand(std::shared_ptr<Merlin::GameScene> scene,
                               Merlin::UUID uuid)
            : m_scene(scene), m_entity_uuid(uuid)
        {
        }

        virtual void Do() override
        {
            auto entity = m_scene->GetEntity(m_entity_uuid);
            m_component_data = entity->GetComponent<T>();
            entity->RemoveComponent<T>();
        }

        virtual void Undo() override
        {
            auto entity = m_scene->GetEntity(m_entity_uuid);
            auto& component = entity->AddComponent<T>();
            component = m_component_data;
        }

        virtual void Redo() override
        {
            auto entity = m_scene->GetEntity(m_entity_uuid);
            entity->RemoveComponent<T>();
        }
    };

    template <typename T>
    class EditComponentCommand : public EditorCommand
    {
        std::shared_ptr<Merlin::GameScene> m_scene;
        Merlin::UUID m_entity_uuid;
        T m_original_component_data;
        T m_changed_component_data;

    public:
        EditComponentCommand(std::shared_ptr<Merlin::GameScene> scene,
                             Merlin::UUID uuid,
                             T changed_component_data)
            : m_scene(scene)
            , m_entity_uuid(uuid)
            , m_component_data(changed_component_data)
        {
        }

        virtual void Do() override
        {
            auto entity = m_scene->GetEntity(m_entity_uuid);
            m_component_data = entity->GetComponent<T>();
            m_original_component_data = m_component_data;
            m_component_data = m_changed_component_data;
        }

        virtual void Undo() override
        {
            auto entity = m_scene->GetEntity(m_entity_uuid);
            m_component_data = entity->GetComponent<T>();
            m_component_data = m_original_component_data;
        }

        virtual void Redo() override
        {
            auto entity = m_scene->GetEntity(m_entity_uuid);
            m_component_data = entity->GetComponent<T>();
            m_component_data = m_changed_component_data;
        }
    };

}  // namespace MerlinEditor
#endif