#include <gtest/gtest.h>
#include "Merlin/Render/cubemap_data.hpp"

using namespace Merlin;


TEST(CubemapDataTest, test_create)
{
    int resolution = 3;
    int channel_count = 4;
    CubemapData data(resolution, channel_count);

    ASSERT_EQ(resolution, data.GetResolution());
    ASSERT_EQ(channel_count, data.GetChannelCount());

    for (int face_id = CubeFace::Begin; face_id < CubeFace::End; face_id += 1)
    {
        auto face = static_cast<CubeFace>(face_id);
        for (int i = 0; i < resolution; ++i)
        {
            for (int j = 0; j < resolution; ++j)
            {
                for (int channel = 0; channel < channel_count; channel++)
                {
                    ASSERT_EQ(0.0f, data.GetPixel(face, i, j, channel));
                }
            }
        }
    }
}

TEST(CubemapDataTest, test_set_pixel)
{
    int resolution = 3;
    int channel_count = 2;
    CubemapData data(resolution, channel_count);

    CubeFace face = CubeFace::PositiveY;
    int i = 1;
    int j = 2;
    int channel = 0;

    ASSERT_EQ(0.0f, data.GetPixel(face, i, j, channel));

    float value = 1.0;
    data.GetPixel(face, i, j, channel) = value;

    ASSERT_EQ(value, data.GetPixel(face, i, j, channel));
}

TEST(CubemapDataTest, test_coordinate_roundtrip)
{
    int resolution = 3;
    int channel_count = 2;
    CubemapData data(resolution, channel_count);

    for (int face_id = CubeFace::Begin; face_id < CubeFace::End; face_id += 1)
    {
        auto face = static_cast<CubeFace>(face_id);
        CubemapCoordinates initial_coordinates{
            face, 0.1f, 0.3f };

        auto cube_point = data.GetPixelCubePoint(
            initial_coordinates);

        auto recovered_coordinates = data.GetPointCoordinates(
            cube_point);

        ASSERT_EQ(initial_coordinates.face, recovered_coordinates.face);
        ASSERT_NEAR(initial_coordinates.u, recovered_coordinates.u, 1.0e-7);
        ASSERT_NEAR(initial_coordinates.v, recovered_coordinates.v, 1.0e-7);
    }
}
