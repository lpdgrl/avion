#ifndef AVION_CORE_ENGINE_H 
#define AVION_CORE_ENGINE_H 1

#include <memory>
#include <string_view>
#include <cstdint>

#include "AvionEngineCore/macro.h"

// Forward declaration
namespace avion::api::backend
{
  class Backend;
}

namespace avion::gfx
{
  class SceneRenderer;  
}

namespace avion::core
{
  class Window;
  class Scene;
  struct Profiler;
  
  namespace resman
  {
    class ResourceManager;
    namespace filewatcher
    {
      class FileWatcher;
    }
  }

  namespace modelmanager
  {
    class ModelManager;
  }

  namespace texturemanager
  {
    class TextureManager;
  }

  namespace common
  {
    class IRenderApp;
    class EventQueue;
  }

  namespace scene
  {
    struct SceneWrapper;
  }
}

namespace avion::controller
{
  class Controller;
}

namespace avion::core::engine
{

  class Engine 
  {
    public:
      using Backend             = api::backend::Backend;
      using ResManager          = resman::ResourceManager;

      using SceneRenderer       = gfx::SceneRenderer;
      using SceneWrapper        = scene::SceneWrapper;

      using Window              = core::Window;
      using ModelManager        = core::modelmanager::ModelManager;
      using TextureManager      = core::texturemanager::TextureManager;

      using IRenderApp          = core::common::IRenderApp;
      using Controller          = avion::controller::Controller;

      Engine();
      Engine(const Engine& other) = delete;
      Engine(Engine&& other) = delete;

      Engine& operator=(const Engine& rhs) = delete;
      Engine& operator=(Engine&& rhs) = delete;

      ~Engine();

      void Init();
      void Run();
      void Render();
      void Shutdown();

      void AddRenderApp(std::unique_ptr<IRenderApp>&& u_ptr);

      // Public API for control Scene
      AV_ATTR_NODISCARD auto AddEntityToScene(std::string_view) -> std::uint64_t;
      AV_ATTR_NODISCARD auto RemoveEntityFromScene(std::uint64_t id) -> bool;
      
      auto GetSceneWrapper() -> SceneWrapper;
    
      auto SaveSceneToJson() -> void;
      auto LoadSceneFromJson(const std::string& name = "scene.json") -> void;

      ResManager&     GetResourceManager();
      SceneRenderer&  GetSceneRenderer(); 
      Profiler&       GetProfiler();
      Scene&          GetScene();
      Window&         GetWindow() noexcept;
      Backend&        GetBackend();
      TextureManager& GetTextureManager() noexcept;
      ModelManager&   GetModelManager() noexcept;
      Controller&     GetController() noexcept;

    private:
      struct EngineImpl;
      std::unique_ptr<EngineImpl> m_impl;
  };
} // namespace avion::core

#endif
