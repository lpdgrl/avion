#include "AvionEngineCore/core/engine.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
  using Engine = avion::core::engine::Engine;
  
  Engine engine;

  engine.Init();
  engine.Run();

  return EXIT_SUCCESS;
}
