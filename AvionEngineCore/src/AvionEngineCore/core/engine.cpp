#include "AvionEngineCore/core/engine.hpp"
#include "AvionEngineCore/core/window.hpp"

namespace avion::core::engine
{
  Engine::Engine()
  : m_resman(std::make_unique<ResManager>(std::filesystem::canonical("/proc/self/exe").c_str()))
  , m_pl_queue(std::make_unique<PipelineQueue>())
  , m_scene(kObjectsCreate, *m_resman.get(), *m_pl_queue.get())
  , m_pipeline(std::make_unique<Pipeline>(m_scene, *m_resman.get(), *m_pl_queue.get(), m_profiler.render_state))
  {
    AV_LOG_INFO("Running avion engine v. " + m_version_engine)
  }

  void Engine::Init(int width, int height)
  {
    m_pipeline->Init(width, height);

    m_scene.AddObjectToScene(ObjectType::kCube, ObjectParams{
      .transform{
        .position{0.f, -1.f, 0.f},
        .size{20.f, 0.05f, 20.f},
      },
      .color{0.5f, 0.f, 0.f},
      .mixing_color{0.f, 0.f, 0.f},
      .material{.ambient{0.5f, 0.f, 0.f}, .diffuse{0.5f, 0.f, 0.f}, .specular{0.f, 0.f, 0.f}, .shininess = 32.f}
    });

    m_scene.AddObjectToScene(ObjectType::kCube, ObjectParams{
      .transform{
        .position{1.f, 0.f, 0.f},
        .size{1.f, 1.f, 1.f},
      },
      .color{1.f, 0.f, 0.f},
      .mixing_color{0.f, 0.f, 0.f},
      .material{.ambient{1.f, 0.f, 0.f}, .diffuse{1.f, 1.f, 1.f}, .specular{0.f, 0.f, 0.f}, .shininess = 32.f}
    });

    m_scene.AddObjectToScene(ObjectType::kPyramid, ObjectParams{
      .transform{
        .position{-2.f, 0.f, 0.f},
        .size{1.f, 1.f, 1.f},
      },
      .color{0.f, 0.5f, 0.f},
      .mixing_color{0.f, 0.f, 0.f},
      .material{.ambient{0.f, 0.5f, 0.f}, .diffuse{0.f, 0.5f, 0.f}, .specular{0.f, 0.f, 0.f}, .shininess = 32.f}
    });

    m_scene.AddModel("backpack.obj");

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

  void Engine::Loop(Engine::Window* p_window)
  {
    p_window->GlEnable();
    while(!p_window->WindowShouldClose())
    {
      p_window->DeltaTimeUpdate();
      p_window->ProcessEvents();

    
      p_window->ClearColorGl(0.2f, 0.2f, 0.2f);
      Render();

      p_window->SwapBuffers();
      p_window->PollEvents();
    }
  }
} // namespace avion::core
