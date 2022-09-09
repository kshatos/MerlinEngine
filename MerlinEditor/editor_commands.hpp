#ifndef EDITOR_COMMANDS_HPP
#define EDITOR_COMMANDS_HPP
#include <Merlin/Scene/game_scene.hpp>
#include <memory>
#include <queue>

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
        std::queue<std::shared_ptr<EditorCommand>> m_undo_queue;
        std::queue<std::shared_ptr<EditorCommand>> m_redo_queue;

    public:
        void AddCommand(std::shared_ptr<EditorCommand> command);
        void DoCommands();
        void UndoCommand();
        void RedoCommand();
    };

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

}  // namespace MerlinEditor
#endif