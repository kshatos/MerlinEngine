#include <gtest/gtest.h>
#include "Merlin/Render/buffer_data.hpp"

using namespace Merlin;


TEST(BufferData, test_get_set)
{
    BufferLayout layout{
        {ElementDataType::Float2, "float2"},
        {ElementDataType::Float3, "float3"},
        {ElementDataType::Float4, "float4"},
        {ElementDataType::Mat3, "mat3"},
        {ElementDataType::Mat4, "mat4"},
        {ElementDataType::Int, "int"}
    };

    BufferData block(layout);
    {
        std::string name = "float2";
        auto set_data = glm::vec2(1.0, 2.0);
        block.SetUniformFloat2(name, set_data);
        auto get_data = block.GetUniformFloat2(name);
        for (int i = 0; i < 2; ++i)
        {
            ASSERT_EQ(set_data[i], get_data[i]);
        }
    }
    {
        std::string name = "float3";
        auto set_data = glm::vec3(1.0, 2.0, 3.0);
        block.SetUniformFloat3(name, set_data);
        auto get_data = block.GetUniformFloat3(name);
        for (int i = 0; i < 3; ++i)
        {
            ASSERT_EQ(set_data[i], get_data[i]);
        }
    }
    {
        std::string name = "float4";
        auto set_data = glm::vec4(1.0, 2.0, 3.0, 4.0);
        block.SetUniformFloat4(name, set_data);
        auto get_data = block.GetUniformFloat4(name);
        for (int i = 0; i < 4; ++i)
        {
            ASSERT_EQ(set_data[i], get_data[i]);
        }
    }
    {
        std::string name = "mat3";
        auto set_data = glm::mat3{
            {1.0, 2.0, 3.0},
            {4.0, 5.0, 6.0},
            {7.0, 8.0, 9.0}
        };
        block.SetUniformMat3(name, set_data);
        auto get_data = block.GetUniformMat3(name);
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                ASSERT_EQ(set_data[i][j], get_data[i][j]);
    }
    {
        std::string name = "mat4";
        auto set_data = glm::mat4{
            {1.0, 2.0, 3.0, 4.0},
            {5.0, 6.0, 7.0, 8.0},
            {9.0, 10.0, 11.0, 12.0},
            {13.0, 14.0, 15.0, 16.0}
        };
        block.SetUniformMat4(name, set_data);
        auto get_data = block.GetUniformMat4(name);
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                ASSERT_EQ(set_data[i][j], get_data[i][j]);
    }
}

TEST(BufferData, test_no_overwrite)
{
    BufferLayout layout{
        {ElementDataType::Float2, "float2"},
        {ElementDataType::Float3, "float3"},
    };
    glm::vec2 vec2_set_data = glm::vec2(1.0, 2.0);
    glm::vec3 vec3_set_data = glm::vec3(3.0, 4.0, 5.0);

    BufferData block(layout);
    block.SetUniformFloat2("float2", vec2_set_data);
    block.SetUniformFloat3("float3", vec3_set_data);
    auto vec2_get_data = block.GetUniformFloat2("float2");
    auto vec3_get_data = block.GetUniformFloat3("float3");

    for (int i = 0; i < 2; i++)
        ASSERT_EQ(vec2_set_data[i], vec2_get_data[i]);
    for (int i = 0; i < 3; i++)
        ASSERT_EQ(vec3_set_data[i], vec3_get_data[i]);
}