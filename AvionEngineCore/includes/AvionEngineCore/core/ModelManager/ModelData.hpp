#ifndef AVION_CORE_MODEL_MANAGER_MODEL_DATA_H
#define AVION_CORE_MODEL_MANAGER_MODEL_DATA_H 1

  #include <vector>
  #include <array>
  #include <map>
  #include <string>
  #include <filesystem>
  #include <cstdint>

  #include "glm/glm.hpp"

  #include "AvionEngineCore/core/TextureManager/TextureHandler.hpp"
  #include "AvionEngineCore/core/Animation/Bone.hpp"

  namespace avion::core::modelmanager::detail
  {
    static constexpr int kMaxBoneInfluence = 4;
    struct Vertex
    {
      glm::vec3 position{};
      glm::vec3 normal{};
      glm::vec2 tex_coords{};

      // bone indexes which influence this vertex
      std::array<int, kMaxBoneInfluence> bone_ids;
      // weights from each bone
      std::array<float, kMaxBoneInfluence> weights;
    };

    struct BoneInfo
    {
      // id in final_bones_matrix
      int id{};

      // offset matrix for transformation from model space to bone space
      glm::mat4 offset;
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
      std::uint32_t first_index{};
      // count indices for mesh
      std::uint32_t index_count{};

      // offset for vertex
      std::uint32_t first_vertex{};
      // count vertex for mesh
      std::uint32_t vertex_count{};
    };
    

    struct CpuModelData
    {
      using Bone           = animation::Bone;
      using AssimpNodeData = animation::AssimpNodeData;

      std::vector<Vertex> vertices;
      std::vector<unsigned int> indices;
      std::vector<MeshRange> mesh_range;
      std::vector<TextureSource> texture_source;
      
      // Animation
      bool has_animation{};
      int ticks_per_second{};
      int bone_counter{};
      float duration{};
      std::vector<animation::Bone> bones;
      animation::AssimpNodeData root_node;
      std::map<std::string, BoneInfo> bone_info_map;
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
