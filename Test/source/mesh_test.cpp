#include <gtest/gtest.h>
#include "Merlin/Render/mesh.hpp"

using namespace Merlin;


TEST(MeshTest, test_size)
{
    Mesh<float> mesh;

    ASSERT_EQ(mesh.GetVertexCount(), 0);
    mesh.SetVertexCount(5);
    ASSERT_EQ(mesh.GetVertexCount(), 5);

    ASSERT_EQ(mesh.GetTriangleCount(), 0);
    mesh.SetTriangleCount(3);
    ASSERT_EQ(mesh.GetTriangleCount(), 3);
}

TEST(MeshTest, test_get_set_data)
{
    Mesh<float> mesh;

    mesh.SetVertexCount(5);
    for (int i = 0; i < 5; ++i)
    {
        ASSERT_EQ(mesh.GetVertex(i), 0);
        mesh.GetVertex(i) = (float)i;
        ASSERT_EQ(mesh.GetVertex(i), i);
    }

    mesh.SetTriangleCount(4);
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            ASSERT_EQ(mesh.GetIndex(i, j), 0);
            mesh.GetIndex(i, j) = j;
            ASSERT_EQ(mesh.GetIndex(i, j), j);
        }
    }
}

TEST(MeshTest, test_set_data_array)
{
    float vertices[] { 0.0f, 1.0f, 2.0 };
    uint32_t indices[] { 0, 1, 2 };

    Mesh<float> mesh;
    
    mesh.SetVertexData(vertices, sizeof(vertices) / sizeof(float));
    ASSERT_EQ(mesh.GetVertexCount(), 3);
    for (int i = 0; i < 3; ++i)
        ASSERT_EQ(mesh.GetVertex(i), vertices[i]);
    

    mesh.SetIndexData(indices, sizeof(indices) / sizeof(uint32_t));
    ASSERT_EQ(mesh.GetTriangleCount(), 1);
    for (int i = 0; i < 1; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            ASSERT_EQ(mesh.GetIndex(i, j), indices[3 * i + j]);
        }
    }


}
