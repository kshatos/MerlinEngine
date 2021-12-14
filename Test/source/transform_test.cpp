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

TEST(TransformTest, test_scale_vector)
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

TEST(TransformTest, test_frame_vectors)
{
    Transform t;
    ASSERT_EQ(t.Forward(), glm::vec3(0.0, 0.0, -1.0));
    ASSERT_EQ(t.Up(), glm::vec3(0.0, 1.0, 0.0));
    ASSERT_EQ(t.Right(), glm::vec3(1.0, 0.0, 0.0));

    t.Rotate(glm::vec3(0.0, 1.0, 0.0), glm::pi<float>()/2.0);

    auto forward_error = t.Forward() - glm::vec3(-1.0, 0.0, 0.0);
    auto up_error = t.Up() - glm::vec3(0.0, 1.0, 0.0);
    auto right_error = t.Right() - glm::vec3(0.0, 0.0, -1.0);

    ASSERT_NEAR(glm::dot(forward_error, forward_error), 0.0f, 1.0e-6);
    ASSERT_NEAR(glm::dot(up_error, up_error), 0.0f, 1.0e-6);
    ASSERT_NEAR(glm::dot(right_error, right_error), 0.0f, 1.0e-6);
}

TEST(TransformTest, test_look_at)
{
    Transform t;

    t.LookAt(glm::vec3(-2.0, 0.0, 0.0));

    auto forward_error = t.Forward() - glm::vec3(-1.0, 0.0, 0.0);
    auto right_error = t.Right() - glm::vec3(0.0, 0.0, -1.0);
    auto up_error = t.Up() - glm::vec3(0.0, 1.0, 0.0);

    ASSERT_NEAR(glm::length(forward_error), 0.0f, 1.0e-6f);
    ASSERT_NEAR(glm::length(right_error), 0.0f, 1.0e-6f);
    ASSERT_NEAR(glm::length(up_error), 0.0f, 1.0e-6f);
}

TEST(TransformTest, test_rotate_around)
{
    Transform t;
    t.Translate(glm::vec3(1.0f, 0.0f, 0.0f));
    t.RotateAround(
        glm::vec3(0.0),
        glm::vec3(0.0, 1.0, 0.0),
        glm::pi<float>() / 2.0);


    auto forward_error = t.Forward() - glm::vec3(-1.0f, 0.0f, 0.0f);
    auto right_error = t.Right() - glm::vec3(0.0f, 0.0f, -1.0f);
    auto up_error = t.Up() - glm::vec3(0.0f, 1.0f, 0.0f);
    auto position_error = t.GetPosition() - glm::vec3(0.0f, 0.0f, -1.0f);

    ASSERT_NEAR(glm::length(forward_error), 0.0f, 1.0e-6f);
    ASSERT_NEAR(glm::length(right_error), 0.0f, 1.0e-6f);
    ASSERT_NEAR(glm::length(up_error), 0.0f, 1.0e-6f);
    ASSERT_NEAR(glm::length(position_error), 0.0f, 1.0e-6f);
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
