#include <gtest/gtest.h>
#include "Merlin/Render/texture2d_data.hpp"

using namespace Merlin;


TEST(Texture2DDataTest, test_create)
{
    uint32_t nx = 3;
    uint32_t ny = 4;
    uint32_t nc = 2;

    Texture2DData data(nx, ny, nc);

    ASSERT_EQ(nx, data.GetXResolution());
    ASSERT_EQ(ny, data.GetYResolution());
    ASSERT_EQ(nc, data.GetChannelCount());

    for (int i = 0; i < nx; ++i)
    {
        for (int j = 0; j < ny; ++j)
        {
            for (int c = 0; c < nc; ++c)
            {
                ASSERT_EQ(0.0f, data.GetPixel(i, j, c));
            }
        }
    }
}

TEST(Texture2DDataTest, test_set_pixel)
{
    uint32_t nx = 3;
    uint32_t ny = 4;
    uint32_t nc = 2;

    Texture2DData data(nx, ny, nc);

    int i = 1, j = 2, c = 0;
    float value = 1.0f;

    ASSERT_EQ(0.0f, data.GetPixel(i, j, c));

    data.GetPixel(i, j, c) = value;

    ASSERT_EQ(value, data.GetPixel(i, j, c));
}
