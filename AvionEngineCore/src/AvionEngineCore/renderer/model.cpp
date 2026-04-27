#include "AvionEngineCore/renderer/model.hpp"
#include "AvionEngineCore/core/resource_manager.hpp"

namespace avion::gfx
{

  Model::Model(const std::string& path, const std::string& filename, ResManager& resman)
  : m_filename(filename)
  , m_path(path)
  , m_resman(resman)
  {

  }

  bool Model::LoadModel()
  {
    AV_LOG_DEBUG("Model::LoadModel");

    Assimp::Importer importer;
    std::string path;
    path += m_path + "/" + m_filename;

    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
      std::string error;
      error += "Assimp::";
      error += importer.GetErrorString();
      AV_LOG_ERROR(error);
      return false;
    }

    ProcessNode(scene->mRootNode, scene);

    return true;
  }

  void Model::ProcessNode(aiNode *node, const aiScene *scene)
  {
    // AV_LOG_DEBUG("Model::ProcessNode");

    // process all the node's meshes
    for (size_t i = 0; i < node->mNumMeshes; ++i)
    {
      aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
      m_meshes.push_back(ProcessMesh(mesh, scene));
    }

    // then do the same for each of its children
    for (size_t i = 0; i < node->mNumChildren; ++i)
    { 
      ProcessNode(node->mChildren[i], scene);
    }
  }

  Mesh Model::ProcessMesh(aiMesh *mesh, const aiScene *scene)
  {
    // AV_LOG_DEBUG("Model::ProcessMesh");

    std::vector<Vertex_t>     vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture_t>    textures;

    // process vertices of mesh
    for (size_t i = 0; i < mesh->mNumVertices; ++i)
    {
      Vertex_t vertex;
      
      glm::vec3 vector;
      auto& ai_vertex = mesh->mVertices[i];
      vector.x = ai_vertex.x;
      vector.y = ai_vertex.y;
      vector.z = ai_vertex.z;
      vertex.position = vector;

      if (mesh->HasNormals())
      {
        auto& ai_normal = mesh->mNormals[i];
        vector.x = ai_normal.x;
        vector.y = ai_normal.y;
        vector.z = ai_normal.z;
        vertex.normal = vector;
      }

      if (mesh->mTextureCoords[0])
      {
        glm::vec2 vector2d;
        vector2d.x = mesh->mTextureCoords[0][i].x;
        vector2d.y = mesh->mTextureCoords[0][i].y;
        vertex.texture_coordinates = vector2d;
      }
      else
      {
        vertex.texture_coordinates = glm::vec2(0.f, 0.f);
      }

      vertices.push_back(vertex);
    }

    // process indices
    for (size_t i = 0; i < mesh->mNumFaces; ++i)
    {
      aiFace& face = mesh->mFaces[i];
      for (size_t j = 0; j < face.mNumIndices; ++j)
      {
        indices.push_back(face.mIndices[j]);
      }
    }
    if (mesh->mMaterialIndex >= 0)
    {
      aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
      std::vector<Texture_t> diffuse_map  = LoadMaterialTextures(material, aiTextureType_DIFFUSE);
      textures.insert_range(textures.begin(), diffuse_map);
  
      std::vector<Texture_t> specular_map = LoadMaterialTextures(material, aiTextureType_SPECULAR);
      textures.insert_range(textures.end(), specular_map);
    }
  
    return Mesh(std::move(vertices), std::move(indices), std::move(textures));
  }

  std::vector<Texture_t> Model::LoadMaterialTextures(aiMaterial *mat, aiTextureType type)
  {
    // AV_LOG_DEBUG("Model::LoadMaterialTextures");

    std::vector<Texture_t> textures;
    AV_LOG_DEBUG("Model::LoadMaterialTextures: " + std::to_string(mat->GetTextureCount(type)));

    for (size_t i = 0; i < mat->GetTextureCount(type); ++i)
    {
      aiString str;
      mat->GetTexture(type, i, &str);
      
      std::string name_texture(str.C_Str());
      
      if (core::Texture* p_tex = m_resman.GetResource<core::Texture>(name_texture); p_tex != nullptr)
      {
        Texture_t texture;
        texture.id = 0;
        texture.type = ConvertAiTextureType(type);
        texture.ptr_texture = p_tex;
        textures.push_back(texture);
        continue;
      }
      
      std::string path;
      path = m_path + "/" + name_texture;
      core::Texture* ptr_tex = m_resman.RegisterTexture(path);

      Texture_t texture;
      texture.id   = 0;
      texture.type = ConvertAiTextureType(type);
      texture.ptr_texture = ptr_tex;
      textures.push_back(texture);
    }

    return textures;
  }

  TextureType Model::ConvertAiTextureType(aiTextureType type) const noexcept
  {
    // AV_LOG_DEBUG("Model::ConvertAiTextureType");
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

  std::vector<Mesh>& Model::GetMeshs() noexcept
  {
    return m_meshes;
  }
} // namespace avion::gfx
