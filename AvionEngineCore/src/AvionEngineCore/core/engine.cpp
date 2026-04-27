#include "AvionEngineCore/core/engine.hpp"

namespace avion::core
{
  Engine::Engine(const char* name, int width, int height)
  : m_window(std::make_unique<Window>(name, width, height))
  {
    AV_LOG_INFO("Avion Engine v. " + m_version_engine)
  }

  void Engine::Init()
  {
    m_window->Init();
  }

  void Engine::Update()
  {
    m_window->Update();
  }

  void Engine::Shutdown()
  {

  }

} // namespace avion::core