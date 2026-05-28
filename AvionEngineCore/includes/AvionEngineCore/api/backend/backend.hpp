#ifndef AVION_API_BACKEND_H
#define AVION_API_BACKEND_H

  #include "AvionEngineCore/api/opengl/opengl_renderer.hpp"

  namespace avion::api::backend
  {
    enum class Api
    {
      kUnknown  = -1,
      kOpengl   = 0,
      kVulkan   = 1,
      kDirectX  = 2,
    };

    class Backend final 
    {
      public:
        using OpenglRenderer = backend::opengl::OpenglRenderer;

        Backend() = delete;
        explicit Backend(Api api);
        
        Backend(const Backend&) = delete;
        Backend(Backend&&)      = delete;

        Backend& operator=(const Backend&) = delete;
        Backend& operator=(Backend&&)      = delete;
        
        ~Backend() = default;

      protected:

      private:
        OpenglRenderer m_ogl_renderer;
        Api m_api;
    };
  }

#endif 
