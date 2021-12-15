#ifndef CUBEMAP_DATA_HPP
#define CUBEMAP_DATA_HPP
#include <array>
#include <vector>
#include <glm/glm.hpp>


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

    class CubemapData
    {
        std::vector<float> m_data;
        uint32_t m_channel_count;
        uint32_t m_resolution;
    public:
        CubemapData(uint32_t resolution, uint32_t channel_count);

        inline uint32_t GetChannelCount() { return m_channel_count; }
        inline uint32_t GetResolution() { return m_resolution; }
        float& GetPixel(
            CubeFace face,
            uint32_t i,
            uint32_t j,
            uint32_t channel);

        glm::vec3 GetPixelCubePoint(CubeFace face, uint32_t i, uint32_t j);
        float* GetFaceDataPointer(CubeFace face);
    private:
        uint32_t CubemapData::PixelIndex(
            CubeFace face,
            uint32_t i,
            uint32_t j,
            uint32_t channel);
    };

}
#endif
