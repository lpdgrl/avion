#include <gtest/gtest.h>

#include <string_view>

#include "AvionEngineCore/core/scene.hpp"
#include "AvionEngineCore/core/SceneSubsystem/SceneWrapper.hpp"

#include "AvionEngineCore/core/ModelManager/ModelManager.hpp"
#include "AvionEngineCore/core/TextureManager/TextureManager.hpp"

// GLOBAL VARIABLE !!!
constexpr std::size_t number_objects{10};
avion::core::modelmanager::ModelManager mm;
avion::core::texturemanager::TextureManager tm;

TEST(Scene, CreateEntity)
{
  using namespace avion::core;

  {
    std::uint64_t model_id{100};
    Scene scene(number_objects, mm, tm);
    auto entity_id = scene.CreateEntity("object1", model_id);
    EXPECT_EQ(entity_id, 1);

    {
      std::uint64_t return_model_id = scene.GetModelID(entity_id).value_or(0);
      EXPECT_EQ(return_model_id, model_id);
    }
    
    AV_ATTR_MAYBE_UNUSED std::uint64_t id{};
    for (std::size_t i = 1; i <= 10; ++i)
    {
      std::string name_object("object");
      name_object.append(std::to_string(i));
      id = scene.CreateEntity(name_object, i);
    }

    std::uint64_t exp_model_id = 10;
    std::uint64_t return_model_id = scene.GetModelID(id).value_or(0);
    EXPECT_EQ(exp_model_id, return_model_id);

    std::uint64_t exp_entity_id = 11;
    EXPECT_EQ(id, exp_entity_id);
  }
}

TEST(Scene, GetModelID)
{
  using namespace avion::core;

  {
    std::uint64_t model_id{100};
    Scene scene(number_objects, mm, tm);
    auto entity_id = scene.CreateEntity("object1", model_id);

    EXPECT_EQ(scene.GetModelID(entity_id).value_or(0), model_id);

    AV_ATTR_MAYBE_UNUSED std::uint64_t id{};
    for (std::size_t i = 1; i <= 10; ++i)
    {
      std::string name_object("object");
      name_object.append(std::to_string(i));
      id = scene.CreateEntity(name_object, i);
      std::uint64_t exp_model_id = scene.GetModelID(id).value_or(0);
      EXPECT_EQ(exp_model_id, i);
    }

    std::uint64_t exp_model_id = 10;
    std::uint64_t return_model_id = scene.GetModelID(id).value_or(0);
    EXPECT_EQ(exp_model_id, return_model_id);
  }
}

