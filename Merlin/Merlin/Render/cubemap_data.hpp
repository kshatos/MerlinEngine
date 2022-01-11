#ifndef CUBEMAP_DATA_HPP
#define CUBEMAP_DATA_HPP
#include <array>
#include <vector>
#include <glm/glm.hpp>
#include "Merlin/Render/cubemap.hpp"
#include "Merlin//Render/mesh_utils.hpp"

namespace Merlin
{
    struct CubemapCoordinates
    {
        CubeFace face;
        float u;
        float v;

        CubemapCoordinates(
            CubeFace _face,
            float _u,
            float _v) :
            face(_face),
            u(_u),
            v(_v)
        {
        }
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
        glm::vec3 GetPixelCubePoint(
            CubeFace face,
            uint32_t i,
            uint32_t j);
        glm::vec3 GetPixelCubePoint(
            CubemapCoordinates coordinates);
        CubemapCoordinates GetPointCoordinates(
            glm::vec3 point);
        TangentFrame GetCubeTangentFrame(
            CubeFace face,
            uint32_t i,
            uint32_t j);

        float* GetFaceDataPointer(
            CubeFace face);

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
