#include "AvionEngineCore/core/engine.hpp"
#include "AvionEngineCore/core/Engine/EngineImpl.hpp"

namespace avion::core::engine
{

  Engine::Engine()
  : m_impl(std::make_unique<EngineImpl>())
  {

  }

  Engine::~Engine()
  {

  }

  void Engine::Init()
  {
    m_impl->InitImpl();
  }

  void Engine::Run()
  {
    m_impl->RunImpl();
  }

  void Engine::Render()
  {
    m_impl->RenderImpl();
  }

  void Engine::Shutdown()
  {
    m_impl->ShutdownImpl();
  }

  void Engine::AddRenderApp(std::unique_ptr<IRenderApp>&& u_ptr)
  {
    m_impl->AddRenderAppImpl(std::move(u_ptr));
  }
  
  auto Engine::AddEntityToScene(std::string_view name) -> bool
  {
    auto result = m_impl->AddEntityToSceneImpl(name);
    return result;
  }

  auto Engine::SaveSceneToJson() -> void
  {
    m_impl->SaveSceneToJsonImpl();
  }

  auto Engine::LoadSceneFromJson(const std::string& name) -> void
  {
    m_impl->LoadSceneFromJsonImpl(name);
  }

  Engine::ResManager& Engine::GetResourceManager()
  {
    return m_impl->GetResourceManagerImpl();
  }

  Engine::SceneRenderer& Engine::GetSceneRenderer()
  {
    return m_impl->GetSceneRendererImpl();
  }

  Profiler& Engine::GetProfiler()
  {
    return m_impl->GetProfilerImpl();
  }

  Scene& Engine::GetScene()
  {
    return m_impl->GetSceneImpl();
  }

  Engine::Window& Engine::GetWindow() noexcept
  {
    return m_impl->GetWindowImpl();
  }

  Engine::Backend& Engine::GetBackend()
  {
    return m_impl->GetBackendImpl();
  }

  Engine::TextureManager& Engine::GetTextureManager() noexcept
  {
    return m_impl->GetTextureManagerImpl();
  }

  Engine::ModelManager& Engine::GetModelManager() noexcept
  {
    return m_impl->GetModelManagerImpl();
  }

  Engine::Controller& Engine::GetController() noexcept
  {
    return m_impl->GetControllerImpl();
  }

} // namespace avion::core
