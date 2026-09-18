#ifndef AVION_API_BACKEND_H
#define AVION_API_BACKEND_H 1

  #include <string>
  #include <cassert>
  #include <deque>
  #include <vector>

  #include "AvionEngineCore/api/backend/RenderItem.hpp"
  #include "AvionEngineCore/api/backend/renderstate.hpp"
  #include "AvionEngineCore/api/opengl/opengl_renderer.hpp"

  #include "AvionEngineCore/core/resource_manager.hpp"
  #include "AvionEngineCore/core/profiler.hpp"
  #include "AvionEngineCore/macro.h"

  #include "AvionEngineCore/core/Common/EventQueue.hpp"

  namespace avion::api::backend
  {
    namespace detail
    {
      enum class RenderAPI
      {
        kUnknown  = 0,
        kOpengl   = 1,
        kVulkan   = 2,
      };

      enum class Projection
      {
        kOrthographic = 0,
        kPerspective  = 1,
        kUnknown      = 2,
      };

      std::string ApiToString(RenderAPI api) noexcept;
    } // namespace avion::api::backend::detail
    
    class Backend final 
    {
      public:
        using BackgroundColor = detail::ColorState;
        using ViewportState   = detail::ViewportState;
        using DepthState      = detail::DepthState;
        using StencilState    = detail::StencilState;
        using BlendState      = detail::BlendingState;
        using RenderState     = detail::RenderState;
        using RenderAPI       = detail::RenderAPI; 
        using Projection      = detail::Projection;
        using RenderItem      = detail::RenderItem;
        using LightSource     = detail::LightSrcRenderable;
        using RenderStatistics = core::RenderStatistics;

        using RenderableQueue = std::deque<RenderItem>;

        using ShaderStorage   = gfx::ShaderStorage;

        using ResManager      = core::resman::ResourceManager;
        using CpuModelData       = core::modelmanager::detail::CpuModelData;
        using ModelHandler    = core::modelmanager::detail::ModelHandler;
        using VertexModel     = core::modelmanager::detail::Vertex;

        using TextureHandler  = core::texturemanager::detail::TextureHandler;
        using TextureData     = core::texturemanager::detail::TextureData;
        using FrameBufferId   = std::uint32_t;

        using EventChangedFiles = core::common::EventQueue::EventChangedFiles;

        Backend() = delete;
        explicit Backend(RenderAPI api, ResManager& resman, RenderStatistics& rnd_stat);

        Backend(const Backend&) = delete;
        Backend(Backend&&)      = delete;

        Backend& operator=(const Backend&) = delete;
        Backend& operator=(Backend&&)      = delete;
        
        ~Backend() = default;

        void Init(RenderState render_state); 

        void BeginFrame();
        void EndFrame(); 
        void UpdateFrame();
        void ApplyDefaultRenderState();

        void Draw();
        void SubmitRenderableItem(RenderItem item) noexcept;

        // GPU Objects
        ModelHandler CreateModel(CpuModelData& cpu_model_data);
        TextureHandler CreateTexture2D(const TextureData& data);
        void CreateFrameBuffer(const std::string& name, float width, float height);

        auto& GetFrameBuffer(const std::string& name) noexcept
        {
          auto* ptr = static_cast<opengl::OpenglRenderer*>(m_renderer.get());
          return ptr->GetFrameBuffer(name);
        }

        // Change current state
        bool SetColorState(BackgroundColor bg_color) noexcept;
        bool SetDepthState(DepthState state) noexcept;
        bool SetStencilState(StencilState state) noexcept;
        bool SetBlendingState(BlendState state) noexcept;
        bool SetViewportState(ViewportState viewport) noexcept;
        bool SetRenderState() noexcept;

        // Change projection
        template <typename... Args>
        void SetProjection(Projection projection, Args&&... args);

        std::string GetNameApi() const noexcept;
        auto ReloadChangedShaders(const std::vector<EventChangedFiles>& events) noexcept -> void;

      protected:

      private:
        void CompileAndLoadShaders();

      private:
        ResManager& m_resman;
        ShaderStorage m_shader_storage;
        // TODO: It's must be interface class for different api (opengl or vulkan) render
        std::unique_ptr<IRenderer> m_renderer;

        RenderState m_current_state;
        RenderState m_default_state;
        RenderAPI m_api;

        RenderableQueue m_renderable_queue;

        RenderStatistics& m_render_stat;

        bool m_is_dirty_state = false;
    };

    template <typename... Args>
    void Backend::SetProjection(Projection projection, Args&&... args)
    {
      switch (projection)
      {
        case Projection::kOrthographic:
        {
          detail::OrthoProjection projection(std::forward<Args>(args)...);
          m_renderer->SetOrthoProjection(projection);

          break;
        }
        case Projection::kPerspective:
        {
          detail::PerspectiveProjection projection(std::forward<Args>(args)...);
          m_renderer->SetPerspectiveProjection(projection);
          
          break;
        }
        case Projection::kUnknown:
        {
          AV_LOG_ERROR("Backend::SetProjection: projection is unknown");
          break;
        }
      }
    }
  } 

#endif 
