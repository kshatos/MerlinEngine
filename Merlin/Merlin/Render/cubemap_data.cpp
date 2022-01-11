#include "Merlin/Render/cubemap_data.hpp"


namespace Merlin
{
    CubemapData::CubemapData(
        uint32_t resolution,
        uint32_t channel_count) :
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

    glm::vec3 CubemapData::GetPixelCubePoint(
        CubeFace face,
        uint32_t i,
        uint32_t j)
    {
        float u = (i + 0.5f) / m_resolution;
        float v = (j + 0.5f) / m_resolution;
        return GetPixelCubePoint(CubemapCoordinates(face, u, v));
    }

    glm::vec3 CubemapData::GetPixelCubePoint(
        CubemapCoordinates coordinates)
    {
        float u = 2.0f * coordinates.u - 1.0f;
        float v = 2.0f * coordinates.v - 1.0f;
        switch (coordinates.face)
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

    CubemapCoordinates CubemapData::GetPointCoordinates(
        glm::vec3 point)
    {
        // Project to sphere
        glm::vec3 sphere_point = glm::normalize(point);

        // Get largest component
        float absx = glm::abs(sphere_point.x);
        float absy = glm::abs(sphere_point.y);
        float absz = glm::abs(sphere_point.z);

        CubeFace face;
        if (absx > absy && absx > absz)
            face = point.x > 0.0f ? CubeFace::PositiveX : CubeFace::NegativeX;
        else if (absy > absx && absy > absz)
            face = point.y > 0.0f ? CubeFace::PositiveY : CubeFace::NegativeY;
        else
            face = point.z > 0.0f ? CubeFace::PositiveZ : CubeFace::NegativeZ;

        // Project point back to cube
        float u = 0.0f;
        float v = 0.0f;
        switch (face)
        {
        case CubeFace::PositiveX:
            sphere_point *= +1.0f / sphere_point.x;
            u = -sphere_point.z;
            v = -sphere_point.y;
            break;
        case CubeFace::NegativeX:
            sphere_point *= -1.0f / sphere_point.x;
            u = +sphere_point.z;
            v = -sphere_point.y;
            break;
        case CubeFace::PositiveY:
            sphere_point *= +1.0f / sphere_point.y;
            u = sphere_point.x;
            v = sphere_point.z;
            break;
        case CubeFace::NegativeY:
            sphere_point *= -1.0f / sphere_point.y;
            u = +sphere_point.x;
            v = -sphere_point.z;
            break;
        case CubeFace::PositiveZ:
            sphere_point *= +1.0f / sphere_point.z;
            u = +sphere_point.x;
            v = -sphere_point.y;
            break;
        case CubeFace::NegativeZ:
            sphere_point *= -1.0f / sphere_point.z;
            u = -sphere_point.x;
            v = -sphere_point.y;
            break;
        }
        u = 0.5f * (u + 1.0f);
        v = 0.5f * (v + 1.0f);

        return CubemapCoordinates(face, u, v);
    }

    float* CubemapData::GetFaceDataPointer(
        CubeFace face)
    {
        auto index = PixelIndex(face, 0, 0, 0);
        return &m_data[index];
    }

    std::shared_ptr<Cubemap> UploadCubemap(
        std::shared_ptr<CubemapData> data)
    {
        auto cubemap = Cubemap::Create(data->GetResolution(), data->GetChannelCount());

        for (uint32_t face_id = CubeFace::Begin; face_id < CubeFace::End; ++face_id)
        {
            auto face = static_cast<CubeFace>(face_id);
            cubemap->SetFaceData(face, data->GetFaceDataPointer(face));
        }

        return cubemap;
    }

}