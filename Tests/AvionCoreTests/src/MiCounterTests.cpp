#include <gtest/gtest.h>

#include <cstdint>
#include <numeric>

#include "AvionEngineCore/core/Common/MiCounter.hpp"

using namespace avion::core;

TEST(MiCounter, DefaultConstructor)
{
  common::MiCounter<std::uint64_t> counter;
  EXPECT_EQ(counter.GetMaxValue(), std::numeric_limits<std::uint64_t>::max());
  EXPECT_EQ(counter.GetUnchangedValue(), 0);
}

TEST(MiCounter, OneIncrease)
{
  common::MiCounter<std::uint64_t> counter;
  EXPECT_EQ(counter.GetValueAfterIncrease(), 1UL);
  EXPECT_EQ(counter.GetUnchangedValue(), 1UL);
}

TEST(MiCounter, LoopIncreaseToMax)
{
  common::MiCounter<std::uint16_t> counter;
  std::uint16_t max = std::numeric_limits<std::uint16_t>::max();
  for (std::uint16_t i = 1; i < max; ++i)
  {
    EXPECT_EQ(counter.GetValueAfterIncrease(), i);
  }
  EXPECT_EQ(counter.GetValueAfterIncrease(), max);
}
