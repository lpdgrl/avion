#include <gtest/gtest.h>
#include "../../AvionMath/include/Vector3.hpp"

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