#ifndef AVION_API_BACKEND_H
#define AVION_API_BACKEND_H

  #include "AvionEngineCore/api/backend/renderstate.hpp"
  #include "AvionEngineCore/api/backend/render_command.hpp"

  #include <string>
  #include <deque>

  namespace avion::api::backend
  {
    enum class RenderAPI
    {
      kUnknown  = -1,
      kOpengl   = 0,
      kVulkan   = 1,
      kDirectX  = 2,
    };

    class Backend final 
    {
      public:
        using RenderState    = detail::RenderState;
        using OpenglRenderer = opengl::OpenglRenderer;

        
        using RenderCommand    = detail::RenderCommand;
        
        Backend() = delete;
        explicit Backend(RenderAPI api, RenderState init_state);
        
        Backend(const Backend&) = delete;
        Backend(Backend&&)      = delete;

        Backend& operator=(const Backend&) = delete;
        Backend& operator=(Backend&&)      = delete;
        
        ~Backend() = default;

        void BeginFrame();
        void EndFrame(); 
        void UpdateFrame();
        void ApplyDrawCommand();
        void ApplyState();

        std::string GetApiString() const noexcept;

      protected:

      private:
        RenderState     m_render_state;
        // TODO: It's must be interface class for different api (opengl or vulkan) render
        OpenglRenderer  m_ogl_renderer;
        RenderAPI       m_api;
        std::deque<RenderCommand> m_queue_draw_commands;
    };
  } 

#endif 
