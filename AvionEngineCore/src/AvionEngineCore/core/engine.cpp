#include "AvionEngineCore/core/engine.hpp"


namespace avion::core::engine
{
  Engine::Engine()
  : m_resman(std::make_unique<ResManager>(std::filesystem::canonical("/proc/self/exe").c_str()))
  , m_scene(kObjectsCreate, m_model_manager)
  , m_scene_renderer(std::make_unique<SceneRenderer>(m_scene, *m_resman.get()))
  , m_window(std::make_unique<Window>("Sandbox", 1920, 1080, m_profiler, m_scene.GetCameraProxy()))
  , m_backend(std::make_unique<Backend>(RenderAPI::kOpengl, *m_resman.get()))
  {
    AV_LOG_INFO("Running avion engine v. " + m_version_engine);
    SettingInternalCallbacks();
  }

  void Engine::SettingInternalCallbacks()
  {
    // Set resource manager callback
    m_model_manager.SetResmanCallback([&resman = *m_resman.get()] (auto filename_model) 
      { 
        using FsPath = resman::ResourceManager::FsPath;
        return resman.GetResource<FsPath>(filename_model);
      }
    );

    // Set backend callback
    m_model_manager.SetBackendCallback([&backend = *m_backend.get()] (auto& model_data)
      {
        return backend.CreateModel(model_data);
      }
    );

    // Set texture manager callback
    m_model_manager.SetTextureManagerCallback([&texture = m_texture_manager] (const auto& path)
      {
        return texture.Load(path);
      }
    );

    // Set backend callback
    m_scene_renderer->SetBackendCallback([&backend = *m_backend.get()] (const auto& render_item) -> void
      {
        return backend.SubmitRenderableItem(render_item);
      }
    );

    // Set resource manager callback
    m_texture_manager.SetResmanCallback([&resman = *m_resman.get()] (auto& path_texture) -> std::optional<Texture*>
      {
        return {resman.RegisterTexture(path_texture)};
      }
    );

    // Set backend callback for texture manager
    m_texture_manager.SetBackendCallback([&backend = *m_backend.get()] (const auto& data)
      {
        return backend.CreateTexture2D(data);
      }
    );
  }

  void Engine::Init()
  {
    using namespace api::backend::detail;

    auto [width_w, height_w] = m_window->GetSize();
    Backend::RenderState render_state{
      .depth_state{.enabled = true, .depth_mask = true},
      .stencil_state{.enabled = true, .stencil_ref = 1, .stencil_func_mask = 0xFF },
      .blend_state{},
      .viewport_state{0, 0, static_cast<std::size_t>(width_w), static_cast<std::size_t>(height_w)},
      .color_state{0.0f, 0.0f, 0.0f, 1.f},
    };

    m_resman->RegisterResource(core::resman::ResourceType::kTexture, "assets/textures");
    m_resman->RegisterResource(core::resman::ResourceType::kShader,  "assets/shaders");
    m_resman->RegisterResource(core::resman::ResourceType::kModel,   "assets/models");
    m_resman->RegisterResource(core::resman::ResourceType::kConfig,  "assets/config");
    m_resman->RegisterResource(core::resman::ResourceType::kSprite,  "assets/sprites");

    m_window->Init();
    m_backend->Init(render_state);
    m_backend->SetProjection(Projection::kPerspective, 45.f, width_w, height_w, 0.1f, 100.f);

    m_scene_renderer->Init(width_w, height_w);
    // m_scene.AddModel("backpack.obj");
    // m_scene.AddPrimitive(Scene::PrimitiveType::kCube);
    // m_scene.AddPrimitive(Scene::PrimitiveType::kPyramid);
    // m_scene.AddPrimitive(Scene::PrimitiveType::kPlane);
    m_scene.AddSourceLight(core::LightType::kDirLight);
  }

  void Engine::Render()
  {
    m_backend->BeginFrame();
    for (auto&& app_item : m_render_app_container)
    {
      app_item->Render();
    }
    m_backend->EndFrame();
  }

  void Engine::Run()
  {
    Loop();
  }

  void Engine::Shutdown()
  {
    AV_LOG_TODO("Engine::Shutdown(): TO DO NOTHING!")
  }

  void Engine::AddRenderApp(std::unique_ptr<IRenderApp> u_ptr)
  {
    m_render_app_container.emplace_back(std::move(u_ptr));
  }

  Engine::ResManager& Engine::GetResourceManager()
  {
    return *m_resman.get();
  }

  Engine::SceneRenderer& Engine::GetSceneRenderer()
  {
    return *m_scene_renderer.get();
  }

  Scene& Engine::GetScene()
  {
    return m_scene;
  }

  Profiler& Engine::GetProfiler()
  {
    return m_profiler;
  }

  Engine::Backend& Engine::GetBackend() 
  { 
    return *m_backend.get(); 
  }

  void Engine::Loop()
  {
    while(!m_window->WindowShouldClose())
    {
      m_window->DeltaTimeUpdate();
      
      Render();
      m_window->SwapBuffers();
      m_window->PollEvents();
      m_window->ProcessEvents();
    }
  }

  Engine::Window& Engine::GetWindow() noexcept
  {
    return *m_window.get();
  }

  Engine::TextureManager& Engine::GetTextureManager() noexcept
  {
    return m_texture_manager;
  }
} // namespace avion::core
