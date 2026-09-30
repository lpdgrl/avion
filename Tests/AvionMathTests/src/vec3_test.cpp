#include <cmath>

#include <gtest/gtest.h>

#include "../../../AvionMath/include/Vector3.hpp"

using Vec3f = av::math::vec::Vector3<float>;

TEST(Vector3f, DefaultConstruction)
{
  constexpr Vec3f vec3f;

  EXPECT_FLOAT_EQ(vec3f.x, 0.0f);
  EXPECT_FLOAT_EQ(vec3f.y, 0.0f);
  EXPECT_FLOAT_EQ(vec3f.z, 0.0f);

  static_assert(vec3f.x == 0.0f);
  static_assert(vec3f.y == 0.0f);
  static_assert(vec3f.z == 0.0f);
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
  auto length = av::math::vec::Length(vec3f);


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
  auto length_squared = av::math::vec::LengthSquared(vec3f);

  EXPECT_FLOAT_EQ(length_squared, 6.f);
}

TEST(Vector3F, NormalizeVectorOperation)
{
  Vec3f vec3f(1.f, 2.f, 1.f);
  auto length = vec3f.Normalize().Length();

  EXPECT_FLOAT_EQ(length, 1.f);

  {
    Vec3f vec3f(0.f, 0.f, 0.f);
    auto length_zero = vec3f.Normalize().Length();
    EXPECT_FLOAT_EQ(length_zero, 0.f);
  }

}

TEST(Vector3F, NormalizeFreedomFunction)
{
  Vec3f vec3f(1.f, 2.f, 1.f);
  av::math::vec::Normalize(vec3f);
  auto length = vec3f.Length();
  
  EXPECT_FLOAT_EQ(length, 1.f);

  Vec3f v2(0.f, 0.f, 0.f);
  auto length_zero = v2.Normalize().Length();
  EXPECT_FLOAT_EQ(length_zero, 0.f);
}

TEST(Vector3F, DotProduct)
{
  {  
    Vec3f v1(0.f, 0.f, 0.f);
    Vec3f v2(0.f, 0.f, 0.f);

    auto dot = v1.Dot(v2);
    EXPECT_FLOAT_EQ(dot, 0.f);
  }

  {  
    Vec3f v1(0.f, 0.f, 0.f);
    Vec3f v2(1.f, 1.f, 1.f);

    auto dot = v1.Dot(v2);
    EXPECT_FLOAT_EQ(dot, 0.f);
  }

  {  
    Vec3f v1(3.f, 4.f, 5.f);
    Vec3f v2(1.f, 1.f, 2.f);

    auto dot = v1.Dot(v2);
    EXPECT_FLOAT_EQ(dot, 17.f);

    v2.Normalize();
    EXPECT_FLOAT_EQ(v1.Dot(v2), 6.94022093789f);
  }

  {  
    Vec3f v1(3.f, 4.f, 5.f);
    Vec3f v2(1.f, 1.f, 2.f);

    v1.Normalize();
    v2.Normalize();
    EXPECT_FLOAT_EQ(v1.Dot(v2), 0.981495457622f);
  }

  {  
    Vec3f v1(3.f, 4.f, 5.f);
    Vec3f v2(1.f, 1.f, 2.f);

    v1.Normalize();
    v2.Normalize();
    // This angle in radians
    EXPECT_FLOAT_EQ(std::acos(v1.Dot(v2)), 0.19267544f);
  }
}

TEST(Vector3F, DotProductFreedomFunction)
{
  {  
    Vec3f v1(0.f, 0.f, 0.f);
    Vec3f v2(0.f, 0.f, 0.f);

    auto dot = av::math::vec::Dot(v1, v2);
    EXPECT_FLOAT_EQ(dot, 0.f);
  }

  {  
    Vec3f v1(0.f, 0.f, 0.f);
    Vec3f v2(1.f, 1.f, 1.f);

    auto dot = av::math::vec::Dot(v1, v2);
    EXPECT_FLOAT_EQ(dot, 0.f);
  }

  {  
    Vec3f v1(3.f, 4.f, 5.f);
    Vec3f v2(1.f, 1.f, 2.f);

    auto dot = av::math::vec::Dot(v1, v2);
    EXPECT_FLOAT_EQ(dot, 17.f);

    v2.Normalize();
    EXPECT_FLOAT_EQ(av::math::vec::Dot(v1, v2), 6.94022093789f);
  }

  {  
    Vec3f v1(3.f, 4.f, 5.f);
    Vec3f v2(1.f, 1.f, 2.f);

    v1.Normalize();
    v2.Normalize();
    EXPECT_FLOAT_EQ(av::math::vec::Dot(v1, v2), 0.981495457622f);
  }

  {  
    Vec3f v1(3.f, 4.f, 5.f);
    Vec3f v2(1.f, 1.f, 2.f);

    v1.Normalize();
    v2.Normalize();
    // This angle in radians
    EXPECT_FLOAT_EQ(std::acos(av::math::vec::Dot(v1, v2)), 0.19267544f);
  }
}

TEST(Vector3F, CrossProduct)
{
  {
    Vec3f v1(0,0,0);
    Vec3f v2(0,0,0);

    auto v3 = v1.Cross(v2);
    auto v4 = v2.Cross(v1);

    EXPECT_FLOAT_EQ(v3.x, 0.f);
    EXPECT_FLOAT_EQ(v3.y, 0.f);
    EXPECT_FLOAT_EQ(v3.z, 0.f);

    EXPECT_FLOAT_EQ(v4.x, 0.f);
    EXPECT_FLOAT_EQ(v4.y, 0.f);
    EXPECT_FLOAT_EQ(v4.z, 0.f);
  }

  {
    Vec3f v1(1,0,0);
    Vec3f v2(0,1,0);

    auto v3 = v1.Cross(v2);
    auto v4 = v2.Cross(v1);

    EXPECT_FLOAT_EQ(v3.x, 0.f);
    EXPECT_FLOAT_EQ(v3.y, 0.f);
    EXPECT_FLOAT_EQ(v3.z, 1.f);

    EXPECT_FLOAT_EQ(v4.x, 0.f);
    EXPECT_FLOAT_EQ(v4.y, 0.f);
    EXPECT_FLOAT_EQ(v4.z, -1.f);
  }
}

TEST(Vector3F, CrossProductFreedomFunction)
{
  {
    Vec3f v1(0,0,0);
    Vec3f v2(0,0,0);

    auto v3 = av::math::vec::Cross(v1, v2);
    auto v4 = av::math::vec::Cross(v2, v1);

    EXPECT_FLOAT_EQ(v3.x, 0.f);
    EXPECT_FLOAT_EQ(v3.y, 0.f);
    EXPECT_FLOAT_EQ(v3.z, 0.f);

    EXPECT_FLOAT_EQ(v4.x, 0.f);
    EXPECT_FLOAT_EQ(v4.y, 0.f);
    EXPECT_FLOAT_EQ(v4.z, 0.f);
  }

  {
    Vec3f v1(1,0,0);
    Vec3f v2(0,1,0);

    auto v3 = av::math::vec::Cross(v1, v2);
    auto v4 = av::math::vec::Cross(v2, v1);

    EXPECT_FLOAT_EQ(v3.x, 0.f);
    EXPECT_FLOAT_EQ(v3.y, 0.f);
    EXPECT_FLOAT_EQ(v3.z, 1.f);

    EXPECT_FLOAT_EQ(v4.x, 0.f);
    EXPECT_FLOAT_EQ(v4.y, 0.f);
    EXPECT_FLOAT_EQ(v4.z, -1.f);
  }
}

TEST(Vector3F, OperatorAdditionFreedomFunction)
{
  {
    Vec3f v1(0,0,0);
    Vec3f v2(0,0,0);

    auto v3 = v1 + v2;

    EXPECT_FLOAT_EQ(v3.x, 0.f);
    EXPECT_FLOAT_EQ(v3.y, 0.f);
    EXPECT_FLOAT_EQ(v3.z, 0.f);
  }

  {
    Vec3f v1(1,0,0);
    Vec3f v2(0,1,0);

    auto v3 = v1 + v2;

    EXPECT_FLOAT_EQ(v3.x, 1.f);
    EXPECT_FLOAT_EQ(v3.y, 1.f);
    EXPECT_FLOAT_EQ(v3.z, 0.f);
  }
}

TEST(Vector3F, OperatorSubtractionFreedomFunction)
{
  {
    Vec3f v1(0,0,0);
    Vec3f v2(0,0,0);

    auto v3 = v1 - v2;

    EXPECT_FLOAT_EQ(v3.x, 0.f);
    EXPECT_FLOAT_EQ(v3.y, 0.f);
    EXPECT_FLOAT_EQ(v3.z, 0.f);
  }

  {
    Vec3f v1(1,0,0);
    Vec3f v2(0,1,0);

    auto v3 = v1 - v2;

    EXPECT_FLOAT_EQ(v3.x, 1.f);
    EXPECT_FLOAT_EQ(v3.y, -1.f);
    EXPECT_FLOAT_EQ(v3.z, 0.f);
  }
  {
    Vec3f v1(1,2,3);
    Vec3f v2(1,1,6);

    auto v3 = v1 - v2;

    EXPECT_FLOAT_EQ(v3.x, 0.f);
    EXPECT_FLOAT_EQ(v3.y, 1.f);
    EXPECT_FLOAT_EQ(v3.z, -3.f);
  }
}


TEST(Vector3F, OperatorMultiplicationFreedomFunction)
{
  {
    Vec3f v1(0,0,0);
    float scalar{2.f};

    auto v3 = v1 * scalar;

    EXPECT_FLOAT_EQ(v3.x, 0.f);
    EXPECT_FLOAT_EQ(v3.y, 0.f);
    EXPECT_FLOAT_EQ(v3.z, 0.f);
  }

  {
    Vec3f v1(1,0,0);
    float scalar{2.f};

    auto v3 = v1 * scalar;

    EXPECT_FLOAT_EQ(v3.x, 2.f);
    EXPECT_FLOAT_EQ(v3.y, 0.f);
    EXPECT_FLOAT_EQ(v3.z, 0.f);
  }
  {
    Vec3f v1(1,2,3);
    float scalar{2.f};

    auto v3 = v1 * scalar;

    EXPECT_FLOAT_EQ(v3.x, 2.f);
    EXPECT_FLOAT_EQ(v3.y, 4.f);
    EXPECT_FLOAT_EQ(v3.z, 6.f);
  }
}

TEST(Vector3F, OperatorAdditionWithAssign)
{
  {
    Vec3f v1(0,0,0);
    Vec3f v2(0,0,0);

    v1 += v2;

    EXPECT_FLOAT_EQ(v1.x, 0.f);
    EXPECT_FLOAT_EQ(v1.y, 0.f);
    EXPECT_FLOAT_EQ(v1.z, 0.f);
  }

  {
    Vec3f v1(1,0,0);
    Vec3f v2(0,1,0);

    v1 += v2;

    EXPECT_FLOAT_EQ(v1.x, 1.f);
    EXPECT_FLOAT_EQ(v1.y, 1.f);
    EXPECT_FLOAT_EQ(v1.z, 0.f);
  }
}