#include "AvionEngineCore/core/ModelManager/PrimitiveModel.hpp"
#include "AvionEngineCore/core/Utils/utils.hpp"

namespace avion::core::modelmanager::detail
{

  PrimitiveModel::ModelData PrimitiveModel::Make(PrimitiveType type) noexcept
  {
    switch (type)
    {
      case PrimitiveType::kCube:
      {
        return MakeCube();
      }
      case PrimitiveType::kPyramid:
      {
        return MakePyramid();
      }
      case PrimitiveType::kPlane:
      {
        return MakePlane();
      }
      case PrimitiveType::kUnknown:
      {
        AV_LOG_INFO("PrimitiveModel::MakePrimitiveModel: type is unknown!");
        break;
      }
    }
    return {};
  }

  PrimitiveModel::ModelData PrimitiveModel::MakeCube() noexcept
  {
    std::vector<float> cube
    {
      // positions          // normals           // texture coords
      -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
      0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
      0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
      0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
      -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

      -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
      0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
      0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
      0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
      -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

      -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
      -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
      -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
      -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
      -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
      -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
      0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
      0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
      0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
      0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

      -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
      0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
      0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
      0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

      -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
      0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
      -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
      -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    constexpr std::size_t number_vertex = 24;
    constexpr std::size_t number_indices = 36;

    return Make(cube, number_vertex, number_indices);
  }

  PrimitiveModel::ModelData PrimitiveModel::MakePyramid() noexcept
  {
    std::vector<float> pyramid
    {
      // Base (down)
      -0.5f, -0.5f, -0.5f,   0.f, -1.f,  0.f, 0.f, 0.f,
      0.5f, -0.5f, -0.5f,   0.f, -1.f,  0.f,  1.f, 0.f,
      0.5f, -0.5f,  0.5f,   0.f, -1.f,  0.f,  1.f, 1.f,

      -0.5f, -0.5f, -0.5f,   0.f, -1.f,  0.f, 0.f, 0.f, 
      0.5f, -0.5f,  0.5f,   0.f, -1.f,  0.f,  1.f, 0.f,
      -0.5f, -0.5f,  0.5f,   0.f, -1.f,  0.f, 1.f, 1.f,

      // Front face (towards -Z)
      0.0f,  0.5f,  0.0f,   0.0f,  0.7071f, -0.7071f,  0.f, 0.f,
      -0.5f, -0.5f, -0.5f,   0.0f,  0.7071f, -0.7071f, 1.f, 0.f,
      0.5f, -0.5f, -0.5f,   0.0f,  0.7071f, -0.7071f,  1.f, 1.f,

      // Right face (towards +X)
      0.0f,  0.5f,  0.0f,   0.7071f, 0.7071f, 0.0f, 0.f, 0.f,
      0.5f, -0.5f, -0.5f,   0.7071f, 0.7071f, 0.0f, 1.f, 0.f,
      0.5f, -0.5f,  0.5f,   0.7071f, 0.7071f, 0.0f, 1.f, 1.f,

      // Back face (towards +Z)
      0.0f,  0.5f,  0.0f,   0.0f,  0.7071f,  0.7071f,  0.f, 0.f,
      0.5f, -0.5f,  0.5f,   0.0f,  0.7071f,  0.7071f,  1.f, 0.f, 
      -0.5f, -0.5f,  0.5f,   0.0f,  0.7071f,  0.7071f, 1.f, 1.f,

      // Left face (towards -X)
      0.0f,  0.5f,  0.0f,  -0.7071f, 0.7071f, 0.0f,  0.f, 0.f,
      -0.5f, -0.5f,  0.5f,  -0.7071f, 0.7071f, 0.0f, 1.f, 0.f,
      -0.5f, -0.5f, -0.5f,  -0.7071f, 0.7071f, 0.0f, 1.f, 1.f
    };

    constexpr std::size_t number_vertex  = 6;
    constexpr std::size_t number_indices = 18;

    return Make(pyramid, number_vertex, number_indices);
  }

  PrimitiveModel::ModelData PrimitiveModel::MakePlane() noexcept
  {
    std::vector<float> plane
    {
      // positions          // normals           // texture coords
      -0.5f, 0.0f, 0.f,     0.f, 0.f, 0.f,        0.f, 0.f,
      -0.5f, 0.5f, 0.0f,    0.f, 0.f, 0.f,        1.f, 0.f,
       0.5f, 0.5f, 0.0f,    0.f, 0.f, 0.f,        1.f, 1.f,

       0.5f, 0.f, 0.f,      0.f, 0.f, 0.f,        1.f, 0.f,
      -0.5f, 0.f, 0.f,      0.f, 0.f, 0.f,        0.f, 0.f,
       0.5f, 0.5f,0.f,      0.f, 0.f, 0.f,        1.f, 1.f
    };

    constexpr std::size_t number_vertex  = 4;
    constexpr std::size_t number_indices = 6;
    return Make(plane, number_vertex, number_indices); 
  }

  PrimitiveModel::ModelData PrimitiveModel::Make(
    const std::vector<float> raw_vertices, 
    std::size_t num_vertex, 
    std::size_t num_indices
  ) noexcept
  {
    ModelData data;
    
    std::vector<Vertex> vertices;
    vertices.reserve(num_vertex);

    std::vector<unsigned int> indices;
    indices.reserve(num_indices);

    std::vector<MeshRange> mesh_range;

    MeshRange m_range;
    for (std::size_t i = 0; i < raw_vertices.size(); i += 8)
    {
      if (i % 6 == 0)
      {
        m_range.first_vertex = vertices.size();
        m_range.first_index  = indices.size();
      }
      vertices.emplace_back(
        glm::vec3(raw_vertices[i], raw_vertices[i + 1], raw_vertices[i + 2]),      // position
        glm::vec3(raw_vertices[i + 3], raw_vertices[i + 4], raw_vertices[i + 5]),  // normal
        glm::vec2(raw_vertices[i + 6], raw_vertices[i + 7])                        // uv
      );

      // one triangle for side
      if (i % 6 == 0)
      {
        indices.push_back(m_range.first_vertex + 0UL);
        indices.push_back(m_range.first_vertex + 1UL);
        indices.push_back(m_range.first_vertex + 2UL);
      }
      // two triangle for side
      // indices.push_back(range.first_vertex + 2UL);
      // indices.push_back(range.first_vertex + 3UL);
      // indices.push_back(range.first_vertex + 0UL);

      if (i % 6 == 0)
      {
        m_range.vertex_count = vertices.size() - m_range.first_vertex;
        m_range.index_count  = indices.size() - m_range.index_count;
        mesh_range.emplace_back(m_range);
      }
    }

    data.vertices = std::move(vertices);
    data.indices = std::move(indices);
    data.mesh_range = std::move(mesh_range);
    data.texture_source.emplace_back(detail::TextureSource::TextureType::kSolidColor, 
      std::filesystem::path());

    return data;
  } 

  std::string PrimitiveTypeToString(PrimitiveType type) noexcept
  {
    std::string result;
    switch (type)
    {
      case PrimitiveType::kCube:
      {
        result.append("cube");
        break;
      }
      case PrimitiveType::kPyramid:
      {
        result.append("pyramid");
        break;
      }
      case PrimitiveType::kPlane:
      {
        result.append("plane");
        break;
      }
      case PrimitiveType::kUnknown:
      {
        result.append("kunknown");
        break;
      }
    }

    return result;
  }

} // namespace avion::core::modelmanager::detail