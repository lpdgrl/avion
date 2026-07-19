#ifndef AVION_API_BACKEND_OPENGL_TYPES_OPENGL_FRAMEBUFFER_H
#define AVION_API_BACKEND_OPENGL_TYPES_OPENGL_FRAMEBUFFER_H 1

  #include <string>

  #include "glad/glad.h"

  #include "AvionEngineCore/macro.h"

  namespace avion::api::backend::opengl
  {
    
    class OpenglFrameBuffer
    {
      public:
        OpenglFrameBuffer() = delete;

        OpenglFrameBuffer(float width, float height);
        OpenglFrameBuffer(const OpenglFrameBuffer& other) = default;

        OpenglFrameBuffer(OpenglFrameBuffer&& other) = default;

        OpenglFrameBuffer& operator=(const OpenglFrameBuffer& other) = default;
        OpenglFrameBuffer& operator=(OpenglFrameBuffer&& other) = default;

        ~OpenglFrameBuffer();

        void RescaleFrameBuffer(float width, float height);
        void Bind() const;
        void Unbind() const;

        unsigned int GetWidth() const noexcept { return m_width; }
        unsigned int GetHeight() const noexcept { return m_height; }
        
        unsigned int GetTexture() const noexcept;

        void SetupFrameBuffer();

      private:
        unsigned int m_fbo;
        unsigned int m_rbo;
        unsigned int m_texture;
        unsigned int m_width;
        unsigned int m_height;
    };

  } // namespace avion::api::backend::opengl

#endif