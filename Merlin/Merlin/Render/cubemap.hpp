#ifndef CUBEMAP_HPP
#define CUBEMAP_HPP
#include <memory>
#include <string>
#include <vector>

#include "Merlin/Render/texture.hpp"

namespace Merlin
{
    enum CubeFace : uint32_t
    {
        PositiveX = 0,
        NegativeX,
        PositiveY,
        NegativeY,
        PositiveZ,
        NegativeZ,
        End,
        Begin = PositiveX,
    };

    class Cubemap : public Texture
    {
    public:
        virtual ~Cubemap(){};

        virtual uint32_t GetResolution() = 0;
        virtual uint32_t GetChannelCount() = 0;

        virtual void SetFaceData(CubeFace face, float* data) = 0;
    };
}  // namespace Merlin
#endif