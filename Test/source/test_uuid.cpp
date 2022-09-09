#include <gtest/gtest.h>

#include "Merlin/Core/uuid.hpp"

using namespace Merlin;

TEST(UUIDTest, TestCreate)
{
    UUID x;
    UUID y;
    UUID z;
    // Test ids dont conflict
    ASSERT_FALSE(x == y);
    ASSERT_FALSE(y == z);
    // Test id not the same as initial test run (not deterministic)
    ASSERT_FALSE(x.ToString() == "8bb42ca2-56ec-4ed5-98d2-2b165fcf91d5");

    ASSERT_TRUE(true);
}

TEST(UUIDTest, TestCreateFromString)
{
    std::string uuid_string = "8bb42ca2-56ec-4ed5-98d2-2b165fcf91d5";
    UUID uuid(uuid_string);

    ASSERT_EQ(uuid.ToString(), uuid_string);
}

TEST(UUIDTest, TestNilUUID)
{
    auto nil = UUID::Nil();
    auto non_nil = UUID();
    ASSERT_TRUE(nil.IsNil());
    ASSERT_FALSE(non_nil.IsNil());
}