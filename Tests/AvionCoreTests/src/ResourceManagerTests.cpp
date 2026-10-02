#include <gtest/gtest.h>

#include <string>

#include "AvionEngineCore/core/resource_manager.hpp"

TEST(ResourceManager, NormalizePath)
{
  using namespace avion::core::resman;

  {  
    std::string path = "/home/lpdgrl/Project/code/avion/build/bin/";
    std::string result = "/home/lpdgrl/Project/code/avion/assets";
    EXPECT_EQ(result, NormalizePath(path));
  }
  {  
    std::string path = "/home/lpdgrl/Project/code/avion/build/";
    std::string result = "/home/lpdgrl/Project/code/avion/assets";
    EXPECT_EQ(result, NormalizePath(path));
  }

  {  
    std::string path = "/home/lpdgrl/Project/code/avion/";
    std::string result = "/home/lpdgrl/Project/code/avion/assets";
    EXPECT_EQ(result, NormalizePath(path));
  }

  // TODO: This test broken NormalizePath and it sends to infinity loop
  // {  
  //   std::string path = "/home/lpdgrl/Project/code/";
  //   std::string result = "/home/lpdgrl/Project/code/avion/assets";
  //   EXPECT_EQ(result, NormalizePath(path));
  // }
}