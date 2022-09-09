#ifndef EDITOR_COMMANDS_HPP
#define EDITOR_COMMANDS_HPP
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
        std::queue<std::shared_ptr<EditorCommand>> m_undo_queue;
        std::queue<std::shared_ptr<EditorCommand>> m_redo_queue;

    public:
        void DoCommand(std::shared_ptr<EditorCommand> command);
        void UndoCommand();
        void RedoCommand();
    };

}  // namespace MerlinEditor
#endif