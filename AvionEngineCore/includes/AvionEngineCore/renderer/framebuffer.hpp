#ifndef AVION_GFX_FRAME_BUFFER_H
#define AVION_GFX_FRAME_BUFFER_H

  #include "glad/glad.h"

  #include "AvionEngineCore/macro.h"

  namespace avion::gfx
  {
    
    class FrameBuffer
    {
      public:
        FrameBuffer() = default;
        FrameBuffer(float width, float height);
        FrameBuffer(const FrameBuffer& other_buffer);

        FrameBuffer& operator=(const FrameBuffer& other);
        ~FrameBuffer();

        void RescaleFrameBuffer(float width, float height);
        void Bind() const;
        void Unbind() const;

        inline unsigned int Width() const noexcept { return m_width; }
        inline unsigned int Height() const noexcept { return m_height; }
        
        unsigned int GetFrameTextures() const noexcept;

      private:
        unsigned int m_fbo;
        unsigned int m_texture;
        unsigned int m_rbo;
        unsigned int m_width;
        unsigned int m_height;
    };

  } // namespace avion::gfx

#endif