#include <gtest/gtest.h>
#include "Merlin/Render/transform.hpp"

using namespace Merlin;


TEST(TransformTest, test_create)
{
    auto pos = glm::vec3(1.0f, 2.0f, 3.0f);
    auto sca = glm::vec3(1.0f, 1.0f, 1.0f);
    auto rot = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    auto t_mat = glm::mat4{
        {1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {1.0f, 2.0f, 3.0f, 1.0f},
    };

    Transform t(pos, sca, rot);
    
    ASSERT_EQ(pos, t.GetPosition());
    ASSERT_EQ(sca, t.GetScale());
    ASSERT_EQ(rot, t.GetOrientation());
    ASSERT_EQ(t_mat, t.GetTransformationMatrix());
}

TEST(TransformTest, test_tranlate)
{
    auto direction = glm::vec3(1.0, 2.0, 3.0);
    auto t_mat = glm::mat4{
        {1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {1.0f, 2.0f, 3.0f, 1.0f},
    };

    Transform t;
    t.Translate(direction);

    ASSERT_EQ(direction, t.GetPosition());
    ASSERT_EQ(glm::vec3(1.0f, 1.0f, 1.0f), t.GetScale());
    ASSERT_EQ(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), t.GetOrientation());
    ASSERT_EQ(t_mat, t.GetTransformationMatrix());
}

TEST(TransformTest, test_scale)
{
    auto direction = glm::vec3(1.0, 2.0, 3.0);
    auto t_mat = glm::mat4{
        {1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 2.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 3.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f},
    };

    Transform t;
    t.Scale(direction);

    ASSERT_EQ(glm::vec3(0.0f, 0.0f, 0.0f), t.GetPosition());
    ASSERT_EQ(direction, t.GetScale());
    ASSERT_EQ(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), t.GetOrientation());
    ASSERT_EQ(t_mat, t.GetTransformationMatrix());
}

TEST(TransformTest, test_rotate)
{
    auto direction = glm::quat(0.0, 0.0f, 0.0, 1.0);
    auto t_mat = glm::mat4{
        {-1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, -1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f},
    };

    Transform t;
    t.Rotate(direction);

    ASSERT_EQ(glm::vec3(0.0f, 0.0f, 0.0f), t.GetPosition());
    ASSERT_EQ(glm::vec3(1.0f, 1.0f, 1.0f), t.GetScale());
    ASSERT_EQ(direction, t.GetOrientation());
    ASSERT_EQ(t_mat, t.GetTransformationMatrix());
}

TEST(TransformTest, test_composition)
{
    constexpr float pi = glm::pi<float>();
    float s = glm::sin(pi / 4.0f);
    float c = glm::sin(pi / 4.0f);

    auto pos = glm::vec3(1.0f, 2.0f, 3.0f);
    auto sca = glm::vec3(4.0f, 5.0f, 6.0f);
    auto rot = glm::quat(c, 0.0f, 0.0f, s);
    auto t_mat = glm::mat4{
        {0.0f, 4.0f, 0.0f, 0.0f},
        {-5.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 6.0f, 0.0f},
        {1.0f, 2.0f, 3.0f, 1.0f},
    };


    Transform t(pos, sca, rot);
    auto mat = t.GetTransformationMatrix();
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            ASSERT_NEAR(t_mat[i][j], mat[i][j], 1.0e-6f);
        }
    }
}
