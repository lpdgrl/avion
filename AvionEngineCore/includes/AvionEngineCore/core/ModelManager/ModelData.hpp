#ifndef AVION_CORE_MODEL_MANAGER_MODEL_DATA_H 
#define AVION_CORE_MODEL_MANAGER_MODEL_DATA_H 1

  #include <vector>
  #include <filesystem>
  #include <cstdint>

  #include "glm/glm.hpp"

  #include "AvionEngineCore/core/TextureManager/TextureHandler.hpp"

  namespace avion::core::modelmanager::detail
  {
    struct Vertex
    {
      glm::vec3 position {};
      glm::vec3 normal {};
      glm::vec2 tex_coords {};
    };

    struct TextureSource
    {
      using TextureType = core::texturemanager::detail::TextureType;
      
      TextureType type = TextureType::kUnknown;
      std::filesystem::path path;
    };

    // meta data for mesh 
    struct MeshRange
    {
      // offset for index 
      std::uint32_t first_index = 0;
      // count indices for mesh
      std::uint32_t index_count = 0;
      
      // offset for vertex
      std::uint32_t first_vertex = 0;
      // count vertex for mesh
      std::uint32_t vertex_count = 0;
    };

    struct ModelData
    {
      std::vector<Vertex> vertices;
      std::vector<unsigned int> indices;
      std::vector<MeshRange> mesh_range;
      std::vector<TextureSource> texture_source;
    };

    struct ModelHandler
    {
      std::uint32_t id;
    };

    enum class PrimitiveType : std::uint8_t
    {
      kUnknown = 0,
      kCube    = 1,
      kPyramid = 2,
      kPlane   = 3,
    };
  }

#endif 