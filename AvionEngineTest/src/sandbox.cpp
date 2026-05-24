#include "AvionEngineCore/core/engine.hpp"
#include "AvionEngineCore/core/window.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
  using Engine = avion::core::engine::Engine;
  using Window = avion::core::Window;
  
  Engine engine;
  std::unique_ptr<Window> window = std::make_unique<Window>("Sandbox", 1280, 1024, engine.GetPipeline(), engine.GetProfiler());

  window->Init();
  engine.Init(1280, 1024);

  engine.Loop(window.get());

  return EXIT_SUCCESS;
}