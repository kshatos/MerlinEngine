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

TEST(Texture2DDataTest, test_bilinear_interpolate)
{
    uint32_t nx = 2;
    uint32_t ny = 2;
    uint32_t nc = 1;
    Texture2DData data(nx, ny, nc);

    data.GetPixel(0, 0, 0) = 0.0f;
    data.GetPixel(0, 1, 0) = 1.0f;
    data.GetPixel(1, 0, 0) = 1.0f;
    data.GetPixel(1, 1, 0) = 0.5f;

    // Corners
    ASSERT_NEAR(0.0f, BilinearInterpolate(data, 0.0f, 0.0f, 0), 1.0e-6);
    ASSERT_NEAR(1.0f, BilinearInterpolate(data, 1.0f, 0.0f, 0), 1.0e-6);
    ASSERT_NEAR(1.0f, BilinearInterpolate(data, 0.0f, 1.0f, 0), 1.0e-6);
    ASSERT_NEAR(0.5f, BilinearInterpolate(data, 1.0f, 1.0f, 0), 1.0e-6);

    // Edge Midpoints
    ASSERT_NEAR(0.50f, BilinearInterpolate(data, 0.5f, 0.0f, 0), 1.0e-6);
    ASSERT_NEAR(0.50f, BilinearInterpolate(data, 0.0f, 0.5f, 0), 1.0e-6);
    ASSERT_NEAR(0.75f, BilinearInterpolate(data, 0.5f, 1.0f, 0), 1.0e-6);
    ASSERT_NEAR(0.75f, BilinearInterpolate(data, 1.0f, 0.5f, 0), 1.0e-6);

    // Center
    ASSERT_NEAR(0.625f, BilinearInterpolate(data, 0.5f, 0.5f, 0), 1.0e-6);
}
