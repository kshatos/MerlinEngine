#ifndef CUBEMAP_HPP
#define CUBEMAP_HPP
#include <memory>
#include <vector>
#include <string>
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
        static std::shared_ptr<Cubemap> Create(const std::vector<std::string>& face_paths);
        static std::shared_ptr<Cubemap> Create(uint32_t resolution, uint32_t channel_count);

        virtual ~Cubemap() {};

        virtual uint32_t GetResolution() = 0;
        virtual uint32_t GetChannelCount() = 0;

        virtual void SetFaceData(CubeFace face, float* data) = 0;
    };
}
#endif