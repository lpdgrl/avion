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
        using ModelData         = core::modelmanager::detail::ModelData;
        using LoadModelResult   = std::optional<ModelData>;
        using TextureSource     = core::modelmanager::detail::TextureSource;
        using TextureType       = core::texturemanager::detail::TextureType;
        using LoadTextureSource = std::vector<TextureSource>;

        AssimpModelLoaderImpl() = default;
        
        AssimpModelLoaderImpl(const AssimpModelLoaderImpl&) = delete;
        AssimpModelLoaderImpl(AssimpModelLoaderImpl&&) = delete;

        AssimpModelLoaderImpl& operator=(const AssimpModelLoaderImpl&) = delete;
        AssimpModelLoaderImpl& operator=(AssimpModelLoaderImpl&&) = delete;

        ~AssimpModelLoaderImpl() = default;

        LoadModelResult Load(const std::string& path);

      private:
        void ProcessNode(aiNode *node, const aiScene *scene, ModelData& model_data);
        void ProcessMesh(aiMesh *mesh, const aiScene *scene, ModelData& model_data);

        LoadTextureSource LoadMaterialTextures(aiMaterial *material, aiTextureType type, std::unordered_set<std::string>& duplicate);
        TextureType ConvertAiTextureType(aiTextureType type);
    };
  } // namespace avion::core::assimp::detail

#endif