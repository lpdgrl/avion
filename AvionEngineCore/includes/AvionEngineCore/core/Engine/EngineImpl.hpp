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

#include "AvionEngineCore/core/RenderSubsystem/ResourceRenderSubsystem.hpp"


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

    using ResourceRenderSubsystem = core::rendersubsystem::ResourceRenderSubsystem;
    
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

    ResourceRenderSubsystem m_resource_rs;

    std::string         m_version_engine = "0.0.1";
    bool                m_is_running = false;

    RenderAppContainer m_render_app_container;

    // *************************************************
    // *****************               ***************** 
    // *****************    COMMON     ***************** 
    // *****************               ***************** 
    // ************************************************* 

    void InitImpl();
    void RunImpl();
    void RenderImpl();
    void ShutdownImpl();

    void AddRenderAppImpl(std::unique_ptr<IRenderApp>&& u_ptr);


    // *************************************************
    // *****************               ***************** 
    // ***************** SCENE CONTROL ***************** 
    // *****************               ***************** 
    // ************************************************* 

    AV_ATTR_NODISCARD 
    auto AddEntityToSceneImpl(std::string_view) -> bool;

    auto SaveSceneToJsonImpl() -> void;
    auto LoadSceneFromJsonImpl(const std::string& name) -> void;


    // *************************************************
    // *****************               ***************** 
    // *****************  PUBLIC API   ***************** 
    // *****************               ***************** 
    // ************************************************* 

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

    // *** PREPARE RESOURCE ***
      AV_ATTR_NODISCARD
      auto LoadResource(std::string_view name) -> std::uint64_t; 
  };
}