#include "AvionEngineCore/core/Assimp/AssimpModelLoaderImpl.hpp"


namespace avion::core::assimp::detail
{

  AssimpModelLoaderImpl::LoadModelResult AssimpModelLoaderImpl::Load(const std::string& path)
  {
    LoadModelResult load_model_result; 
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
      std::string error;
      error.append("AssimpModelLoaderImpl::Load: ");
      error.append(importer.GetErrorString());
      AV_LOG_ERROR(error);
      return std::nullopt;
    }

    load_model_result = ModelData();
    ProcessNode(scene->mRootNode, scene, load_model_result.value());

    return load_model_result;
  }

  void  AssimpModelLoaderImpl::ProcessNode(aiNode *node, const aiScene *scene, ModelData& model_data)
  {
    // AV_LOG_DEBUG("AssimpModelLoaderImpl::ProcessNode");

    // process all the node's meshes
    for (size_t i = 0; i < node->mNumMeshes; ++i)
    {
      aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
      ProcessMesh(mesh, scene, model_data);
    }

    // then do the same for each of its children
    for (size_t i = 0; i < node->mNumChildren; ++i)
    { 
      ProcessNode(node->mChildren[i], scene, model_data);
    }
  }

  void AssimpModelLoaderImpl::ProcessMesh(aiMesh *mesh, const aiScene *scene, ModelData& model_data)
  {
    // AV_LOG_DEBUG("AssimpModelLoaderImpl::ProcessMesh");
    using namespace core::modelmanager::detail;

    auto& [vertices, indices, mesh_range, texture_source] = model_data; 

    MeshRange range;
    range.first_vertex = vertices.size();
    range.first_index = indices.size();

    // process vertices of mesh
    for (size_t i = 0; i < mesh->mNumVertices; ++i)
    {
      Vertex vertex;
      auto& ai_vertex = mesh->mVertices[i];
      vertex.position.x = ai_vertex.x;
      vertex.position.y = ai_vertex.y;
      vertex.position.z = ai_vertex.z;

      if (mesh->HasNormals())
      {
        auto& ai_normal = mesh->mNormals[i];
        vertex.normal.x = ai_normal.x;
        vertex.normal.y = ai_normal.y;
        vertex.normal.z = ai_normal.z;
      }

      if (mesh->mTextureCoords[0])
      {
        vertex.tex_coords.x = mesh->mTextureCoords[0][i].x;
        vertex.tex_coords.y = mesh->mTextureCoords[0][i].y;
      }
      else
      {
        vertex.tex_coords = glm::vec2(0.f, 0.f);
      }
      vertices.push_back(vertex);
    }

    // process indices
    for (size_t i = 0; i < mesh->mNumFaces; ++i)
    {
      aiFace& face = mesh->mFaces[i];
      for (size_t j = 0; j < face.mNumIndices; ++j)
      {
        uint32_t local_index = face.mIndices[j];
        uint32_t global_index = range.first_vertex + local_index;
        indices.push_back(global_index);
      }
    }

    // count vertex for mesh
    range.vertex_count = vertices.size() - range.first_vertex;
    // count indices for mesh
    range.index_count = indices.size() - range.first_index;
    mesh_range.push_back(range);

    if (mesh->mMaterialIndex >= 0)
    {
      aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
      std::unordered_set<std::string> duplicate;

      std::vector<TextureSource> diffuse_map = LoadMaterialTextures(material, aiTextureType_DIFFUSE, duplicate);
      texture_source.insert_range(texture_source.begin(), diffuse_map);
  
      std::vector<TextureSource> specular_map = LoadMaterialTextures(material, aiTextureType_SPECULAR, duplicate);
      texture_source.insert_range(texture_source.end(), specular_map);
    }
  }

  AssimpModelLoaderImpl::LoadTextureSource AssimpModelLoaderImpl::LoadMaterialTextures(aiMaterial *material, aiTextureType type, std::unordered_set<std::string>& duplicate)
  {
    // AV_LOG_DEBUG("AssimpModelLoaderImpl::LoadMaterialTextures");
    LoadTextureSource textures;
    
    for (size_t i = 0; i < material->GetTextureCount(type); ++i)
    {
      aiString str;
      material->GetTexture(type, i, &str);

      std::string texture(str.C_Str());
      if (auto [_, result] = duplicate.insert(texture); result)
      {
        textures.emplace_back(ConvertAiTextureType(type), str.C_Str());
      }
      
    }
    return textures;
  }

  AssimpModelLoaderImpl::TextureType AssimpModelLoaderImpl::ConvertAiTextureType(aiTextureType type)
  {
    // AV_LOG_DEBUG("AssimpModelLoaderImpl::ConvertAiTextureType");
    switch(type)
    {
      case aiTextureType_NONE:
      {
        return TextureType::kUnknown;
      }
      case aiTextureType_DIFFUSE:
      {
        return TextureType::kDiffuse;
      }
      case aiTextureType_SPECULAR:
      {
        return TextureType::kSpecular;
      }
    }
  }

  std::unique_ptr<AssimpModelLoaderImpl> MakeAssimpModelLoader() noexcept
  {
    return std::make_unique<AssimpModelLoaderImpl>();
  }

} // namespace avion::core::assimp::detail

