#include "Merlin/Render/cubemap_data.hpp"
#include "Merlin/Render/renderer.hpp"


namespace Merlin
{
    glm::vec3 CubemapData::CubePoint(CubemapCoordinates& coordinates)
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

    CubemapCoordinates CubemapData::PointCoordinates(glm::vec3& point)
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

    CubemapData::CubemapData(
        uint32_t resolution,
        uint32_t channel_count) :
        m_resolution(resolution),
        m_channel_count(channel_count)
    {
        uint32_t size = resolution * resolution * channel_count * 6;
        m_data.resize(size);
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

    CubemapCoordinates CubemapData::GetPixelCoordinates(
        CubeFace face,
        uint32_t i,
        uint32_t j)
    {
        return CubemapCoordinates{
            face,
            (i + 0.5f) / m_resolution,
            (j + 0.5f) / m_resolution
        };
    }

    float* CubemapData::GetFaceDataPointer(
        CubeFace face)
    {
        auto index = PixelIndex(face, 0, 0, 0);
        return &m_data[index];
    }

    uint32_t CubemapData::PixelIndex(
        CubeFace face,
        uint32_t i,
        uint32_t j,
        uint32_t channel)
    {
        return channel + m_channel_count * (i + m_resolution * (j + m_resolution * face));
    }

    float BilinearInterpolate(
        CubemapData& cubemap,
        CubemapCoordinates& coords,
        uint32_t channel)
    {
        int n = cubemap.GetResolution();

        int imin = (int)(coords.u * n - 0.5f);
        int imax = (int)(coords.u * n + 0.5f);
        int jmin = (int)(coords.v * n - 0.5f);
        int jmax = (int)(coords.v * n + 0.5f);

        imin = std::max(0, std::min(n - 1, imin));
        imax = std::max(0, std::min(n - 1, imax));
        jmin = std::max(0, std::min(n - 1, jmin));
        jmax = std::max(0, std::min(n - 1, jmax));

        float umin = (imin + 0.5f) / n;
        float umax = (imax + 0.5f) / n;
        float vmin = (jmin + 0.5f) / n;
        float vmax = (jmax + 0.5f) / n;

        float du = umax - umin;
        float dv = vmax - vmin;

        float u_local = du > 0.0f ? (coords.u - umin) / du : 0.5f;
        float v_local = dv > 0.0f ? (coords.v - vmin) / dv : 0.5f;

        float N00 = (1.0f - coords.u) * (1.0f - coords.v);
        float N01 = (1.0f - coords.u) * coords.v;
        float N10 = coords.u * (1.0f - coords.v);
        float N11 = coords.u * coords.v;

        return (
            N00 * cubemap.GetPixel(coords.face, imin, jmin, channel) +
            N01 * cubemap.GetPixel(coords.face, imin, jmax, channel) +
            N10 * cubemap.GetPixel(coords.face, imax, jmin, channel) +
            N11 * cubemap.GetPixel(coords.face, imax, jmax, channel));
    }

    std::shared_ptr<Cubemap> UploadCubemap(
        std::shared_ptr<CubemapData> data)
    {
        auto api = Renderer::GetAPI();
        auto cubemap = api->CreateCubemap(data->GetResolution(), data->GetChannelCount());

        for (uint32_t face_id = CubeFace::Begin; face_id < CubeFace::End; ++face_id)
        {
            auto face = static_cast<CubeFace>(face_id);
            cubemap->SetFaceData(face, data->GetFaceDataPointer(face));
        }

        return cubemap;
    }

}