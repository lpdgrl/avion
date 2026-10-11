#include "AvionEngineCore/core/engine.hpp"
#include "AvionEngineCore/api/backend/backend.hpp"
#include "AvionEngineCore/core/resource_manager.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
  using ResourceType = avion::core::resman::ResourceType;
  using Engine = avion::core::engine::Engine;
  
  Engine engine;
  engine.GetResourceManager().RegisterResource(ResourceType::kFont, "fonts");

  engine.Init();
  engine.GetBackend().SetColorState({1.f, 1.f, 0.f, 1.f});
  
  engine.Run();

  return EXIT_SUCCESS;
}
