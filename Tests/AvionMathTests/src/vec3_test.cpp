#include <gtest/gtest.h>
#include "../../../AvionMath/include/Vector3.hpp"

using Vec3f = avion::math::Vector3<float>;

TEST(Vector3f, DefaultConstruction)
{
  Vec3f vec3f;

  EXPECT_FLOAT_EQ(vec3f.x, 0.0f);
  EXPECT_FLOAT_EQ(vec3f.y, 0.0f);
  EXPECT_FLOAT_EQ(vec3f.z, 0.0f);
}

TEST(Vector3f, ConstructionFromSingleValue)
{
  Vec3f vec3f(0.5f);

  EXPECT_FLOAT_EQ(vec3f.x, 0.5f);
  EXPECT_FLOAT_EQ(vec3f.y, 0.5f);
  EXPECT_FLOAT_EQ(vec3f.z, 0.5f);
}

TEST(Vector3f, ConstructionFromThreeComponents)
{
  Vec3f vec3f(1.f, 0.5f, 1.24f);

  EXPECT_FLOAT_EQ(vec3f.x, 1.f);
  EXPECT_FLOAT_EQ(vec3f.y, 0.5f);
  EXPECT_FLOAT_EQ(vec3f.z, 1.24f);
}

TEST(Vector3f, LengthOperationFromClass)
{
  Vec3f vec3f(1.f, 2.f, 1.f);
  auto length = vec3f.Length();

  EXPECT_FLOAT_EQ(length, 2.44948974278f);
}

TEST(Vector3f, LengthOperationFromFreedomFunction)
{
  Vec3f vec3f(1.f, 2.f, 1.f);
  auto length = avion::math::Length(vec3f);

  EXPECT_FLOAT_EQ(length, 2.44948974278f);
}

TEST(Vector3F, LengthSquared)
{
  Vec3f vec3f(1.f, 2.f, 1.f);
  auto length_squared = vec3f.LengthSquared();

  EXPECT_FLOAT_EQ(length_squared, 6.f);
}

TEST(Vector3F, LengthSquaredFreedomFunction)
{
  Vec3f vec3f(1.f, 2.f, 1.f);
  auto length_squared = avion::math::LengthSquared(vec3f);

  EXPECT_FLOAT_EQ(length_squared, 6.f);
}

TEST(Vector3F, NormalizeVectorOperation)
{
  Vec3f vec3f(1.f, 2.f, 1.f);
}