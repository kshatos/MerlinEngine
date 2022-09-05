#ifndef MERLIN_EDITOR_APP_HPP
#define MERLIN_EDITOR_APP_HPP

#include <Merlin/Core/core.hpp>


namespace MerlinEditor
{
    class MerlinEditorApp : public Merlin::Application
    {
    public:
        MerlinEditorApp(const Merlin::ApplicationInfo& properties);
    };
}  // namespace MerlinEditor
#endif