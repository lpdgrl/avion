#include "AvionEngineCore/renderer/mesh.hpp"

namespace avion::gfx 
{
  Mesh::Mesh(const Mesh::VertexArray& vertices, const Mesh::IndicesArray& indices, const Mesh::TextureArray& textures)
  : m_vertices(vertices)
  , m_indices(indices)
  , m_textures(textures)
  {

  }

  Mesh::Mesh(Mesh::VertexArray&& vertices, Mesh::IndicesArray&& indices, Mesh::TextureArray&& textures)
  : m_vertices(std::move(vertices))
  , m_indices(std::move(indices))
  , m_textures(std::move(textures))
  {

  }
  
  void Mesh::SetBuffersIndices(BufferVertexIndices indices) noexcept
  {
    m_buffer_vertex_indices = indices;
  }

  BufferVertexIndices Mesh::GetBuffersIndices() const noexcept
  {
    return m_buffer_vertex_indices;
  }

  const Mesh::VertexArray& Mesh::GetVerticesArray() const noexcept
  {
    return m_vertices;
  }

  Mesh::TextureArray& Mesh::GetTexturesArray() noexcept
  {
    return m_textures;
  }

  const Mesh::IndicesArray& Mesh::GetIndicesArray() const noexcept
  {
    return m_indices;
  }

} // namespace avion::gfx
