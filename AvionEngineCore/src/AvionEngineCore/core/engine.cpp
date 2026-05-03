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
    m_scene.AddObjectToScene(ObjectType::kCube, ObjectParams{
      .position{0.f, 0.f, 0.f},
      .size{1.f, 1.f, 1.f},
      .color{1.f, 0.f, 0.f},
      .mixing_color{0.f, 0.f, 0.f},
      .material{.ambient{1.f, 0.f, 0.f}, .diffuse{1.f, 1.f, 1.f}, .specular{0.f, 0.f, 0.f}}
    });

    m_scene.AddSourceLight(LightType::kPointLight);
  }
  void Engine::Render()
  {
    m_pipeline->TransferDataToFrameBuffer();
  }

  void Engine::Run()
  {
    
  }

  void Engine::Shutdown()
  {

  }

  void Engine::CreateFrameBuffer(float width, float height)
  {
    m_frame_buffer = FrameBuffer(width, height);
  }

  Engine::FrameBuffer& Engine::GetFrameBuffer()
  {
    return m_frame_buffer;
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