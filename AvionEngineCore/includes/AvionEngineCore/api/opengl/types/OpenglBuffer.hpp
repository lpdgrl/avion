#ifndef AVION_API_BACKEND_OPENGL_TYPES_OPENGL_BUFFER_H
#define AVION_API_BACKEND_OPENGL_TYPES_OPENGL_BUFFER_H

  #include <cstdint>
  #include <vector>
  #include <bit>
  #include <utility>

  #include "glad/glad.h"

  #include "AvionEngineCore/macro.h"

  namespace avion::api::backend::opengl
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
    // Как сделать так, чтобы абстракция не протекла на вверх?
    class OpenglBuffer
    {
      public:
        OpenglBuffer() = default;

        template <typename MeshData>
        void SetupBuffer(MeshData& mesh_data);

        OpenglBuffer(const OpenglBuffer&) = delete("Copy constructor deleted for OpengBuffer");
        OpenglBuffer(OpenglBuffer&&) noexcept;

        OpenglBuffer& operator=(const OpenglBuffer&) = delete("Copy assign operator deleted for OpenglBuffer");
        OpenglBuffer& operator=(OpenglBuffer&&) noexcept;

        ~OpenglBuffer();

        GLuint GetIdVao() const noexcept { return m_vao; }
        GLuint GetIdVbo() const noexcept { return m_vbo; }
        GLuint GetIdEbo() const noexcept { return m_ebo; }
        GLuint GetIndicesSize() const noexcept { return m_indices_size; }

      private:
        GLuint m_vao{};
        GLuint m_vbo{};
        GLuint m_ebo{};
        GLuint m_indices_size{};
    };

    template <typename MeshData>
    void OpenglBuffer::SetupBuffer(MeshData& mesh_data)
    {
      m_indices_size = mesh_data.indices_size;

      glGenVertexArrays(1, &m_vao);
      glGenBuffers(1, &m_vbo);
      glGenBuffers(1, &m_ebo);

      glBindVertexArray(m_vao);
      glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
      glBufferData(GL_ARRAY_BUFFER, mesh_data.vertices_size * mesh_data.stride_size, &mesh_data.vertices[0], GL_DYNAMIC_DRAW);

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh_data.indices_size * sizeof(unsigned int), &mesh_data.indices[0], GL_DYNAMIC_DRAW);

      // positions
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, mesh_data.stride_size, std::bit_cast<void*>(mesh_data.offset_position)
      );

      // normals
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, mesh_data.stride_size, std::bit_cast<void*>(mesh_data.offset_normals));

      // texture coordinates
      glEnableVertexAttribArray(2);
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, mesh_data.stride_size, std::bit_cast<void*>(mesh_data.offset_tex_coords));

      // bone ids
      glEnableVertexAttribArray(3);
      glVertexAttribIPointer(3, 4, GL_INT, mesh_data.stride_size, std::bit_cast<void*>(mesh_data.offset_bone_ids));

      // weights
      glEnableVertexAttribArray(4);
      glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, mesh_data.stride_size, std::bit_cast<void*>(mesh_data.offset_weights));

      glBindVertexArray(0);
    }

  } // namespace avion::api::backend::opengl

#endif
