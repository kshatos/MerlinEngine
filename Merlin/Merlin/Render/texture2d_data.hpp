#ifndef TEXTURE2D_DATA_HPP
#define TEXTURE2D_DATA_HPP
#include <vector>

namespace Merlin
{
    class Texture2DData
    {
        std::vector<unsigned char> m_data;
        uint32_t m_channel_count;
        uint32_t m_x_resolution;
        uint32_t m_y_resolution;

    public:
        Texture2DData(uint32_t x_resolution,
                      uint32_t y_resolution,
                      uint32_t channel_count);
        Texture2DData(uint32_t x_resolution,
                      uint32_t y_resolution,
                      uint32_t channel_count,
                      unsigned char* data);

        inline uint32_t GetChannelCount() const { return m_channel_count; }
        inline uint32_t GetXResolution() const { return m_x_resolution; }
        inline uint32_t GetYResolution() const { return m_y_resolution; }

        unsigned char& GetPixel(uint32_t i, uint32_t j, uint32_t channel);

        const unsigned char* GetDataPointer() const;

    private:
        uint32_t PixelIndex(uint32_t i, uint32_t j, uint32_t channel);
    };

}  // namespace Merlin
#endif
