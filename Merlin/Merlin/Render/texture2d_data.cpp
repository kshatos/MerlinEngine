#include "Merlin/Render/texture2d_data.hpp"

namespace Merlin
{
    Texture2DData::Texture2DData(uint32_t x_resolution,
                                 uint32_t y_resolution,
                                 uint32_t channel_count)
        : m_x_resolution(x_resolution)
        , m_y_resolution(y_resolution)
        , m_channel_count(channel_count)
        , m_data(std::vector<unsigned char>(
              (x_resolution * y_resolution * channel_count), 0.0f))
    {
    }

    Texture2DData::Texture2DData(uint32_t x_resolution,
                                 uint32_t y_resolution,
                                 uint32_t channel_count,
                                 unsigned char* data)
        : m_x_resolution(x_resolution)
        , m_y_resolution(y_resolution)
        , m_channel_count(channel_count)
        , m_data(std::vector<unsigned char>(
              data, data + (x_resolution * y_resolution * channel_count)))
    {
    }

    unsigned char& Texture2DData::GetPixel(uint32_t i,
                                           uint32_t j,
                                           uint32_t channel)
    {
        auto index = PixelIndex(i, j, channel);
        return m_data[index];
    }

    uint32_t Texture2DData::PixelIndex(uint32_t i, uint32_t j, uint32_t channel)
    {
        return channel + m_channel_count * (i + m_x_resolution * j);
    }

    const unsigned char* Texture2DData::GetDataPointer() const
    {
        return m_data.data();
    }

}  // namespace Merlin