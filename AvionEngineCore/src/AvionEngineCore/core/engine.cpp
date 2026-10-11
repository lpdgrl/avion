#include "AvionEngineCore/core/engine.hpp"

#include "AvionEngineCore/core/scene.hpp"
#include "AvionEngineCore/core/profiler.hpp"

#include "AvionEngineCore/renderer/scene_renderer.hpp"

#include "AvionEngineCore/api/backend/backend.hpp"

#include "AvionEngineCore/core/window.hpp"
#include "AvionEngineCore/core/ModelManager/ModelManager.hpp"
#include "AvionEngineCore/core/TextureManager/TextureManager.hpp"

#include "AvionEngineCore/api/backend/IRenderApp.hpp"

#include "AvionEngineCore/core/Common/EventQueue.hpp"
#include "AvionEngineCore/core/ResourceManager/FileWatcher.hpp"


namespace avion::core::engine
{
  struct Engine::EngineImpl
  {
    static constexpr int kObjectsCreate = 1000;
    
    template <typename T>
    using UPtr = std::unique_ptr<T>;

    using Backend             = api::backend::Backend;
    using RenderAPI           = api::backend::detail::RenderAPI;
    using ResManager          = resman::ResourceManager;

    using SceneRenderer       = gfx::SceneRenderer;

    using Window              = core::Window;
    using ModelManager        = core::modelmanager::ModelManager;
    using TextureManager      = core::texturemanager::TextureManager;

    using IRenderApp          = core::common::IRenderApp;
    using RenderAppContainer  = std::vector<UPtr<IRenderApp>>;

    using EventQueue          = core::common::EventQueue;
    using FileWatcher         = core::resman::filewatcher::FileWatcher;

    using Controller          = avion::controller::Controller;
    
    EngineImpl();
    EngineImpl(const Engine& other) = delete;
    EngineImpl(Engine&& other) = delete;

    EngineImpl& operator=(const Engine& rhs) = delete;
    EngineImpl& operator=(Engine&& rhs) = delete;

    ~EngineImpl();
    
    // TODO: NOW IT ISN'T SAFE THREAD
    // EventManager        m_event_manager;

    TextureManager      m_texture_manager;
    ModelManager        m_model_manager;
    UPtr<ResManager>    m_resman;
    Scene               m_scene;
    Profiler            m_profiler;
    UPtr<Backend>       m_backend;
    UPtr<SceneRenderer> m_scene_renderer;
    UPtr<Window>        m_window;
    EventQueue          m_event_queue;
    FileWatcher         m_file_watcher;

    std::string         m_version_engine = "0.0.1";
    bool                m_is_running = false;

    RenderAppContainer m_render_app_container;

    void InitImpl();
    void RunImpl();
    void RenderImpl();
    void ShutdownImpl();

    void AddRenderAppImpl(std::unique_ptr<IRenderApp>&& u_ptr);

    auto SaveSceneToJsonImpl() -> void;
    auto LoadSceneFromJsonImpl(const std::string& name) -> void;

    ResManager&     GetResourceManagerImpl();
    SceneRenderer&  GetSceneRendererImpl();
    Profiler&       GetProfilerImpl();
    Scene&          GetSceneImpl();
    Window&         GetWindowImpl() noexcept;
    Backend&        GetBackendImpl();
    TextureManager& GetTextureManagerImpl() noexcept;
    ModelManager&   GetModelManagerImpl() noexcept;
    Controller&     GetControllerImpl() noexcept;

    private:
      void LoopImpl();
      void SettingInternalCallbacksImpl();
      void ProcessEventsImpl() noexcept;
  };

  Engine::EngineImpl::EngineImpl()
  : m_resman(std::make_unique<ResManager>(std::filesystem::canonical("/proc/self/exe").c_str()))
  , m_scene(kObjectsCreate, m_model_manager, m_texture_manager)
  , m_scene_renderer(std::make_unique<SceneRenderer>(m_scene, *m_resman.get()))
  , m_window(std::make_unique<Window>("Sandbox", 1920, 1080, m_profiler, m_scene.GetCameraProxy()))
  , m_backend(std::make_unique<Backend>(RenderAPI::kOpengl, *m_resman.get(), m_profiler.render_state.render_stat))
  , m_file_watcher(m_event_queue, *m_resman.get())
  {
    AV_LOG_INFO("Running avion engine v. " + m_version_engine);
    SettingInternalCallbacksImpl();
  }

  Engine::EngineImpl::~EngineImpl()
  {

  }

  void Engine::EngineImpl::SettingInternalCallbacksImpl()
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

    // TODO: This dirty method!!!
    // Set window callback for scene 
    m_scene_renderer->SetWindowCallback([&window = *m_window.get()]() -> float
    {
      return window.GetDeltaTime();
    });
    
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

  void Engine::EngineImpl::InitImpl()
  {
    using namespace api::backend::detail;

    auto [width_w, height_w] = m_window->GetSize();
    Backend::RenderState render_state{
      .depth_state{.enabled = true, .depth_mask = true, .depth_func = CompareFunc::Less},
      .stencil_state{.enabled = true, .stencil_ref = 1, .stencil_func_mask = 0xFF },
      .blend_state{.enabled = true, .blend_source_factor = BlendingFunc::SourceAlpha, .blend_destination_factor = BlendingFunc::OneMinusSourceAlpha},
      .viewport_state{0, 0, static_cast<std::size_t>(width_w), static_cast<std::size_t>(height_w)},
      .color_state{0.0f, 0.0f, 0.0f, 1.f},
    };

    m_window->Init();
    m_backend->Init(render_state);

    m_scene_renderer->Init(width_w, height_w);
  }

  void Engine::EngineImpl::RenderImpl()
  {
    m_backend->BeginFrame();
    for (auto&& app_item : m_render_app_container)
    {
      app_item->Render();
    }
    m_backend->EndFrame();
  }

  void Engine::EngineImpl::RunImpl()
  {
    LoopImpl();
  }

  void Engine::EngineImpl::ShutdownImpl()
  {
    AV_LOG_TODO("Engine::Shutdown(): TO DO NOTHING!")
  }

  void Engine::EngineImpl::AddRenderAppImpl(std::unique_ptr<IRenderApp>&& u_ptr)
  {
    m_render_app_container.emplace_back(std::move(u_ptr));
  }

  Engine::EngineImpl::ResManager& Engine::EngineImpl::GetResourceManagerImpl()
  {
    return *m_resman.get();
  }

  Engine::EngineImpl::SceneRenderer& Engine::EngineImpl::GetSceneRendererImpl()
  {
    return *m_scene_renderer.get();
  }

  Scene& Engine::EngineImpl::GetSceneImpl()
  {
    return m_scene;
  }

  Profiler& Engine::EngineImpl::GetProfilerImpl()
  {
    return m_profiler;
  }

  Engine::EngineImpl::Backend& Engine::EngineImpl::GetBackendImpl() 
  { 
    return *m_backend.get(); 
  }

  Engine::EngineImpl::ModelManager& Engine::EngineImpl::GetModelManagerImpl() noexcept
  {
    return m_model_manager;
  }

  void Engine::EngineImpl::LoopImpl()
  {
    while(!m_window->WindowShouldClose())
    {
      m_window->DeltaTimeUpdate();
      ProcessEventsImpl();

      RenderImpl();
      
      m_window->FramePerSecond();
      m_window->SwapBuffers();
      m_window->PollEvents();
      m_window->ProcessEvents();
    }
  }

  Engine::EngineImpl::Window& Engine::EngineImpl::GetWindowImpl() noexcept
  {
    return *m_window.get();
  }

  Engine::EngineImpl::TextureManager& Engine::EngineImpl::GetTextureManagerImpl() noexcept
  {
    return m_texture_manager;
  }

  void Engine::EngineImpl::ProcessEventsImpl() noexcept
  {
    auto result = m_event_queue.Consume();
    if (!result.has_value())
    {
      return;
    }

    auto& events = result.value();
    for (const auto& event : events)
    {
      AV_LOG_DEBUG(std::format("Engine::ProcessEvents: File {} is changed", event.path.c_str()));
    }
    m_backend->ReloadChangedShaders(events);
  }

  auto Engine::EngineImpl::SaveSceneToJsonImpl() -> void
  {
    [[maybe_unused]] auto result = m_scene.Export();
  }

  auto Engine::EngineImpl::LoadSceneFromJsonImpl(const std::string& name) -> void
  {
    [[maybe_unused]] auto result = m_scene.Import();
  }

  Engine::EngineImpl::Controller& Engine::EngineImpl::GetControllerImpl() noexcept
  {
    return m_window->GetController();
  }

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
