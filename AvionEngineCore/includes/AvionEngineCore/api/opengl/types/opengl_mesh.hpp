#ifndef AVION_API_BACKEND_OPENGL_TYPES_OPENGL_MESH_H
#define AVION_API_BACKEND_OPENGL_TYPES_OPENGL_MESH_H

  #include <cstdint>
  #include <vector>
  #include <bit>

  #include "glad/glad.h"
  #include "../../../macro.h"

  namespace avion::api::backend::types
  {
    struct VertexAttrib
    {
      GLuint        index;
      GLint         size;
      GLenum        type;
      GLboolean     normalized;
      GLsizei       stride;
      const GLvoid  *pointer;
    };
    
    // Что делает????
    // Создает буфферы в памяти гпу
    // Хранит индексы буфферов
    // Как связан будет с моделью?
    // Как сделать так, чтобы абстракция не протекла на вверху?
    class OpenglMesh
    {
      public:
        OpenglMesh();

        template <typename Vertex>
        void SetupMesh(const std::vector<Vertex>& vertices);

        OpenglMesh(const OpenglMesh&);
        OpenglMesh(OpenglMesh&&) = delete;

        OpenglMesh& operator=(const OpenglMesh&);
        OpenglMesh& operator=(OpenglMesh&&) = delete;

        ~OpenglMesh();

        GLuint GetIdVao() const noexcept { return m_vao; }
        GLuint GetIdVbo() const noexcept { return m_vbo; }
        GLuint GetIdEbo() const noexcept { return m_ebo; }

      private:
        GLuint m_vao{};
        GLuint m_vbo{};
        GLuint m_ebo{};
    };

    template <typename Vertex>
    void OpenglMesh::SetupMesh(const std::vector<Vertex>& vertices)
    {
      if (vertices.empty())
      {
        AV_LOG_ERROR("void OpenglMesh::SetupMesh(const std::vector<Vertex>& vertices): vertices is empty!");
        return;
      }

      glBindVertexArray(&m_vao);

      glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
      glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

      // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
      // glBufferData(GL_ELEMENT_ARRAY_BUFFER, )

      // positions
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<void*>(0));

      // normals
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), std::bit_cast<void*>(offsetof(Vertex, normal)));

      // texture coordinates
      glEnableVertexAttribArray(2);
      glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), std::bit_cast<void*>(offsetof(Vertex, normal)));

      glBindVertexArray(0);
    }
    
  } // namespace avion::api::backend::types

#endif