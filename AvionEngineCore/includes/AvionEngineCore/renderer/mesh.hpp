#ifndef AVION_GFX_MESH_H 
#define AVION_GFX_MESH_H

  #include <vector>
  #include <string>

  #include "glm/glm.hpp"
  
  #include "AvionEngineCore/core/texture.hpp"

  namespace avion::gfx
  {
    enum class TextureType
    {
      kUnknown  = 0,
      kDiffuse  = 1,
      kSpecular = 2,  
    };

    struct Vertex_t 
    {
      glm::vec3 position;
      glm::vec3 normal;
      glm::vec2 texture_coordinates;
    };

    struct BufferVertexIndices
    {
      unsigned int m_vao = 0;
      unsigned int m_vbo = 0;
      unsigned int m_ebo = 0;
    };

    struct Texture_t
    {
      unsigned int id  = 0;
      TextureType type = TextureType::kUnknown;
      core::Texture* ptr_texture = nullptr;
    };
    
    class Mesh 
    {
      using VertexArray  = std::vector<Vertex_t>;
      using IndicesArray = std::vector<unsigned int>;
      using TextureArray = std::vector<Texture_t>;

      public:
        Mesh() = delete;
        ~Mesh() = default;

        Mesh(const VertexArray& vertices, const IndicesArray& indices, const TextureArray& textures);
        Mesh(VertexArray&& vertices, IndicesArray&& indices, TextureArray&& textures);

        void SetBuffersIndices(BufferVertexIndices vertex_buffer) noexcept;
        BufferVertexIndices GetBuffersIndices() const noexcept;

        const VertexArray& GetVerticesArray() const noexcept;
        TextureArray& GetTexturesArray()      noexcept;
        const IndicesArray& GetIndicesArray() const noexcept;

      private:
        BufferVertexIndices   m_buffer_vertex_indices;
        VertexArray           m_vertices;
        IndicesArray          m_indices;
        TextureArray          m_textures; 
    };
  } // namespace avion::gfx
#endif
