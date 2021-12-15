#include "Merlin/Render/cubemap_data.hpp"


namespace Merlin
{
    CubemapData::CubemapData(uint32_t resolution, uint32_t channel_count) :
        m_resolution(resolution),
        m_channel_count(channel_count)
    {
        uint32_t size = resolution * resolution * channel_count * 6;
        m_data.resize(size);
    }

    uint32_t CubemapData::PixelIndex(
        CubeFace face,
        uint32_t i,
        uint32_t j,
        uint32_t channel)
    {
        return channel + m_channel_count * (i + m_resolution * (j + m_resolution * face));
    }

    float& CubemapData::GetPixel(
        CubeFace face,
        uint32_t i,
        uint32_t j,
        uint32_t channel)
    {
        auto index = PixelIndex(face, i, j, channel);
        return m_data[index];
    }

    glm::vec3 CubemapData::GetPixelCubePoint(CubeFace face, uint32_t i, uint32_t j)
    {
        float u = (i + 0.5f) / m_resolution;
        float v = (j + 0.5f) / m_resolution;
        u = 2.0f * u - 1.0f;
        v = 2.0f * v - 1.0f;
        switch (face)
        {
            case PositiveX:
                return glm::vec3(+1.0f, -v, -u);
            case NegativeX:
                return glm::vec3(-1.0f, -v, +u);
            case PositiveY:
                return glm::vec3(+u, +1.0f, +v);
            case NegativeY:
                return glm::vec3(+u, -1.0f, -v);
            case PositiveZ:
                return glm::vec3(+u, -v, +1.0f);
            case NegativeZ:
                return glm::vec3(-u, -v, -1.0f);
            default:
                return glm::vec3(0.0);
        }
    }

    float* CubemapData::GetFaceDataPointer(CubeFace face)
    {
        auto index = PixelIndex(face, 0, 0, 0);
        return &m_data[index];
    }

}