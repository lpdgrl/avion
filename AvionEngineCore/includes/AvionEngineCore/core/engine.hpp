#ifndef AVION_CORE_ENGINE_H 
#define AVION_CORE_ENGINE_H

  #include "resource_manager.hpp"
  #include "scene.hpp"
  #include "profiler.hpp"

  #include "AvionEngineCore/renderer/pipeline.hpp"
  #include "AvionEngineCore/renderer/pipeline_queue.hpp"

  namespace avion::core::engine
  {

    class Engine 
    {
      public:
        using ResManager    = resman::ResourceManager;
        using PipelineQueue = gfx::PipelineQueue;
        using Pipeline      = gfx::Pipeline;

        template <typename T>
        using UPtr  = std::unique_ptr<T>;

        Engine();

        void Init(int width, int height);
        void Run();
        void Render();
        void Shutdown();

        ResManager& GetResourceManager();
        Pipeline&   GetPipeline();
        Profiler&   GetProfiler();
        Scene&      GetScene();

      private:
        static constexpr int kObjectsCreate = 1000;
        
        UPtr<ResManager>    m_resman;
        UPtr<PipelineQueue> m_pl_queue;
        Scene               m_scene;
        UPtr<Pipeline>      m_pipeline;
        Profiler            m_profiler;
        
        std::string         m_version_engine = "0.0.1";
        bool                m_is_running = false;
    };
  } // namespace avion::core

#endif