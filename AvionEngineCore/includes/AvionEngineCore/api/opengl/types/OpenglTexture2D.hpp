#ifndef AVION_API_BACKEND_OPENGL_TYPES_OPENGL_TEXTURE2D_H
#define AVION_API_BACKEND_OPENGL_TYPES_OPENGL_TEXTURE2D_H

  #include "glad/glad.h"

  #include "AvionEngineCore/macro.h"

  namespace avion::api::backend::opengl
  {
    class OpenglTexture2D
    {
      public:
        using TextureId = GLuint;

        OpenglTexture2D() = default;

        OpenglTexture2D(const OpenglTexture2D&) = delete;
        OpenglTexture2D(OpenglTexture2D&&) = default;

        OpenglTexture2D& operator=(const OpenglTexture2D&) = delete;
        OpenglTexture2D& operator=(OpenglTexture2D&&) = default;

        ~OpenglTexture2D();

        void Bind() const noexcept;
        void UnBind() const noexcept;

        TextureId Get() const noexcept { return m_texture_id; } 

        template <typename TextureItem>
        void SetupTexture(const TextureItem& item) noexcept;

      protected:

      private:
        TextureId m_texture_id;
    };

    template <typename TextureItem>
    void OpenglTexture2D::SetupTexture(const TextureItem& item) noexcept 
    {
      glGenTextures(1, &m_texture_id);
      glBindTexture(GL_TEXTURE_2D, m_texture_id);

      glTexImage2D(GL_TEXTURE_2D, 0, item.color_channels, item.width, item.height, 0, item.color_channels, GL_UNSIGNED_BYTE, &item.buffer[0]);
      glGenerateMipmap(GL_TEXTURE_2D);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      glBindTexture(GL_TEXTURE_2D, 0);
    }
  } // namespace avion::api::backend::opengl

#endif 