#ifndef AVION_CORE_ASSIMP_MODEL_LOADER_IMPL_H
#define AVION_CORE_ASSIMP_MODEL_LOADER_IMPL_H 1

  #include <string>
  #include <memory>
  #include <optional>
  #include <unordered_set>

  #include <assimp/Importer.hpp>
  #include <assimp/scene.h>
  #include <assimp/postprocess.h>

  #include "AvionEngineCore/macro.h"
  #include "AvionEngineCore/core/ModelManager/ModelData.hpp"
  #include "AvionEngineCore/core/TextureManager/TextureHandler.hpp"

  namespace avion::core::assimp::detail
  {
    class AssimpModelLoaderImpl;

    std::unique_ptr<AssimpModelLoaderImpl> MakeAssimpModelLoader() noexcept;

    class AssimpModelLoaderImpl
    {
      public:
        using BoneInfo          = core::modelmanager::detail::BoneInfo;
        using Vertex            = core::modelmanager::detail::Vertex;
        // using ModelData         = core::modelmanager::detail::ModelData;
        // using LoadModelResult   = std::optional<ModelData>;
        using CpuModelData      = core::modelmanager::detail::CpuModelData; 
        using LoadedResultCpuModelData = std::optional<CpuModelData>;
        using TextureSource     = core::modelmanager::detail::TextureSource;
        using TextureType       = core::texturemanager::detail::TextureType;
        using LoadTextureSource = std::vector<TextureSource>;

        AssimpModelLoaderImpl() = default;

        AssimpModelLoaderImpl(const AssimpModelLoaderImpl&) = delete;
        AssimpModelLoaderImpl(AssimpModelLoaderImpl&&) = delete;

        AssimpModelLoaderImpl& operator=(const AssimpModelLoaderImpl&) = delete;
        AssimpModelLoaderImpl& operator=(AssimpModelLoaderImpl&&) = delete;

        ~AssimpModelLoaderImpl() = default;

        LoadedResultCpuModelData Load(const std::string& path);

      private:
        void ProcessNode(aiNode *node, const aiScene *scene, CpuModelData& model_data);
        void ProcessMesh(aiMesh *mesh, const aiScene *scene, CpuModelData& model_data);

        LoadTextureSource LoadMaterialTextures(aiMaterial *material, aiTextureType type, std::unordered_set<std::string>& duplicate);
        TextureType ConvertAiTextureType(aiTextureType type);

        auto SetVertexBoneDataToDefault(Vertex& vertex) noexcept -> void;
        auto SetVertexBoneData(Vertex& vertex, int bone_id, float weight) noexcept -> void;
        auto ExtractBoneWeightForVertices(aiMesh* mesh, const aiScene* scene, CpuModelData& data, std::size_t mesh_first_vertex) noexcept -> void;

        auto ConvertMatrixToGLMFormat(const aiMatrix4x4& from) -> glm::mat4;

        auto ReadHeirarchyData(animation::AssimpNodeData& dest, const aiNode* src) -> void;
        auto ReadMissingBones(const aiAnimation* animation, CpuModelData& model_data) -> void;
    };
  } // namespace avion::core::assimp::detail

#endif
