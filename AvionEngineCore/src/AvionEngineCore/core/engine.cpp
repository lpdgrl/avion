#include "AvionEngineCore/core/engine.hpp"

namespace avion::core::engine
{
  Engine::Engine()
  : m_resman(std::make_unique<ResManager>(std::filesystem::canonical("/proc/self/exe").c_str()))
  , m_pl_queue(std::make_unique<PipelineQueue>())
  , m_scene(kObjectsCreate, *m_resman.get(), *m_pl_queue.get())
  , m_pipeline(std::make_unique<Pipeline>(m_scene, *m_resman.get(), *m_pl_queue.get(), m_profiler.camera_state))
  {
    
    AV_LOG_INFO("Running avion engine v. " + m_version_engine)
  }

  void Engine::Init(int width, int height)
  {
    
    m_pipeline->Init(width, height);
  }

  void Engine::Render()
  {

  }

  void Engine::Run()
  {

  }

  void Engine::Shutdown()
  {

  }

  Engine::ResManager& Engine::GetResourceManager()
  {
    return *m_resman.get();
  }

  Engine::Pipeline& Engine::GetPipeline()
  {
    return *m_pipeline.get();
  }

  Scene& Engine::GetScene()
  {
    return m_scene;
  }

  Profiler& Engine::GetProfiler()
  {
    return m_profiler;
  }

} // namespace avion::core