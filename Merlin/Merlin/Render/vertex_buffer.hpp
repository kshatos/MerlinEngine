#ifndef VERTEX_BUFFER_HPP
#define VERTEX_BUFFER_HPP


namespace Merlin
{
    class VertexBuffer
    {
    public:
        static VertexBuffer* Create(float* vertices, size_t size);

        virtual ~VertexBuffer() {}
        virtual void Bind() = 0;
        virtual void UnBind() = 0;
    };
}

#endif