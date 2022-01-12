#ifndef TEXTURE2D_DATA_HPP
#define TEXTURE2D_DATA_HPP
#include <vector>


namespace Merlin
{
    class Texture2DData
    {
        std::vector<float> m_data;
        uint32_t m_channel_count;
        uint32_t m_x_resolution;
        uint32_t m_y_resolution;
    public:
        Texture2DData(
            uint32_t x_resolution,
            uint32_t y_resolution,
            uint32_t channel_count);

        inline uint32_t GetChannelCount() { return m_channel_count; }
        inline uint32_t GetXResolution() { return m_x_resolution; }
        inline uint32_t GetYResolution() { return m_y_resolution; }

        float& GetPixel(
            uint32_t i,
            uint32_t j,
            uint32_t channel);
    private:

        uint32_t PixelIndex(
            uint32_t i,
            uint32_t j,
            uint32_t channel);
    };
}
#endif
