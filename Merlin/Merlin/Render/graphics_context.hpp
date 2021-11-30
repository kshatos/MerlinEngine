#ifndef GRAPHICS_CONTEXT_HPP
#define GRAPHICS_CONTEXT_HPP


namespace Merlin
{
    class GraphicsContext
    {
    public:
        static GraphicsContext* Create(void* window);

        virtual ~GraphicsContext() {}
        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;
    };
}

#endif
