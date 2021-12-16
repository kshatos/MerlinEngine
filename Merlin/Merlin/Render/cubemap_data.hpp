#ifndef CUBEMAP_DATA_HPP
#define CUBEMAP_DATA_HPP
#include <array>
#include <vector>
#include <glm/glm.hpp>
#include "Merlin/Render/cubemap.hpp"


namespace Merlin
{

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


    std::shared_ptr<Cubemap> UploadCubemap(std::shared_ptr<CubemapData> data);

}
#endif
