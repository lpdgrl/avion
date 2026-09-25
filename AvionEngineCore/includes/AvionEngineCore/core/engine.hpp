#ifndef AVION_CORE_ENGINE_H 
#define AVION_CORE_ENGINE_H

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

  #include <vector>

  namespace avion::core::engine
  {
    class Engine 
    {
      public:
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

        Engine();
        Engine(const Engine& other) = delete;
        Engine(Engine&& other) = delete;

        Engine& operator=(const Engine& rhs) = delete;
        Engine& operator=(Engine&& rhs) = delete;

        ~Engine() = default;

        void Init();
        void Run();
        void Render();
        void Shutdown();

        void AddRenderApp(std::unique_ptr<IRenderApp> u_ptr);

        auto SaveSceneToJson() -> void;
        auto LoadSceneFromJson(const std::string& name = "scene.json") -> void;
 
        ResManager&     GetResourceManager();
        SceneRenderer&  GetSceneRenderer();
        Profiler&       GetProfiler();
        Scene&          GetScene();
        Window&         GetWindow() noexcept;
        Backend&        GetBackend();
        TextureManager& GetTextureManager() noexcept;

      private:
        void Loop();
        void SettingInternalCallbacks();
        void ProcessEvents() noexcept;
  
        static constexpr int kObjectsCreate = 1000;
        
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
    };
  } // namespace avion::core

#endif
