#include "Merlin/Render/texture2d_data.hpp"


namespace Merlin
{
    Texture2DData::Texture2DData(
        uint32_t x_resolution,
        uint32_t y_resolution,
        uint32_t channel_count) :
        m_x_resolution(x_resolution),
        m_y_resolution(y_resolution),
        m_channel_count(channel_count),
        m_data(std::vector<float>((
            x_resolution*
            y_resolution*
            channel_count),
            0.0f))
    {
    }

    float& Texture2DData::GetPixel(
        uint32_t i,
        uint32_t j,
        uint32_t channel)
    {
        auto index = PixelIndex(i, j, channel);
        return m_data[index];
    }

    uint32_t Texture2DData::PixelIndex(
        uint32_t i,
        uint32_t j,
        uint32_t channel)
    {
        return channel + m_channel_count * (i + m_x_resolution * j);
    }


    float* Texture2DData::GetDataPointer()
    {
        return &m_data[0];
    }

    float BilinearInterpolate(
        Texture2DData& texture,
        float u,
        float v,
        uint32_t channel)
    {
        int nx = texture.GetXResolution();
        int ny = texture.GetYResolution();

        int imin = (int)(u * nx - 0.5f);
        int imax = (int)(u * nx + 0.5f);
        int jmin = (int)(v * ny - 0.5f);
        int jmax = (int)(v * ny + 0.5f);

        imin = std::max(0, std::min(nx - 1, imin));
        imax = std::max(0, std::min(nx - 1, imax));
        jmin = std::max(0, std::min(ny - 1, jmin));
        jmax = std::max(0, std::min(ny - 1, jmax));

        float umin = (imin + 0.5f) / nx;
        float umax = (imax + 0.5f) / nx;
        float vmin = (jmin + 0.5f) / ny;
        float vmax = (jmax + 0.5f) / ny;

        float du = umax - umin;
        float dv = vmax - vmin;

        float u_local = du > 0.0f ? (u - umin) / du : 0.5f;
        float v_local = dv > 0.0f ? (v - vmin) / dv : 0.5f;

        float N00 = (1.0f - u) * (1.0f - v);
        float N01 = (1.0f - u) * v;
        float N10 = u * (1.0f - v);
        float N11 = u * v;

        return (
            N00 * texture.GetPixel(imin, jmin, channel) +
            N01 * texture.GetPixel(imin, jmax, channel) +
            N10 * texture.GetPixel(imax, jmin, channel) +
            N11 * texture.GetPixel(imax, jmax, channel));
    }

}