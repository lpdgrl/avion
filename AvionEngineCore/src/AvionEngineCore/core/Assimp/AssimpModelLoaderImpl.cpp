#include "AvionEngineCore/core/Assimp/AssimpModelLoaderImpl.hpp"

#include <algorithm>

namespace avion::core::assimp::detail
{

  AssimpModelLoaderImpl::LoadedResultCpuModelData AssimpModelLoaderImpl::Load(const std::string& path)
  {
    LoadedResultCpuModelData load_model_result;
    load_model_result = CpuModelData();
    
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

    auto& model_data = load_model_result.value();
    ProcessNode(scene->mRootNode, scene, model_data);

    // Read animation
    if (!scene->HasAnimations())
    {
      return load_model_result;
    }
    auto animation = scene->mAnimations[0];
    model_data.has_animation = true;
    model_data.duration = animation->mDuration;
    model_data.ticks_per_second = animation->mTicksPerSecond;

    #if(AV_DEBUG_MODE)
      for (std::size_t i = 0; i < std::min<std::size_t>(20, model_data.vertices.size()); ++i)
      {
        const auto& v = model_data.vertices[i];
        AV_LOG_DEBUG("vertex[" + std::to_string(i) + "]");

        for (int j = 0; j < 4; ++j)
        {
          AV_LOG_DEBUG("bone[" + std::to_string(j) + "] = " + std::to_string(v.bone_ids[j]));
          AV_LOG_DEBUG("weight[" + std::to_string(j) + "] = " + std::to_string(v.weights[j]));
        }
        AV_LOG_DEBUG("weight sum =" + std::to_string(v.weights[0] + v.weights[1] + v.weights[2] + v.weights[3]));
      }
    #endif

    ReadHeirarchyData(model_data.root_node, scene->mRootNode);
    ReadMissingBones(animation, model_data);

    return load_model_result;
  }

  void  AssimpModelLoaderImpl::ProcessNode(aiNode *node, const aiScene *scene, CpuModelData& model_data)
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

  void AssimpModelLoaderImpl::ProcessMesh(aiMesh* mesh, const aiScene* scene, CpuModelData& model_data)
  {
    // AV_LOG_DEBUG("AssimpModelLoaderImpl::ProcessMesh");
    using namespace core::modelmanager::detail;
    
    auto& vertices = model_data.vertices;
    auto& indices = model_data.indices;
    auto& mesh_range = model_data.mesh_range;
    auto& texture_source = model_data.texture_source;

    MeshRange range;
    range.first_vertex = vertices.size();
    range.first_index = indices.size();

    // process vertices of mesh
    for (size_t i = 0; i < mesh->mNumVertices; ++i)
    {
      Vertex vertex;
      SetVertexBoneDataToDefault(vertex);

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
      static std::unordered_set<std::string> duplicate;

      std::vector<TextureSource> diffuse_map = LoadMaterialTextures(material, aiTextureType_DIFFUSE, duplicate);
      texture_source.insert_range(texture_source.begin(), diffuse_map);

      std::vector<TextureSource> specular_map = LoadMaterialTextures(material, aiTextureType_SPECULAR, duplicate);
      texture_source.insert_range(texture_source.end(), specular_map);
    }
    if (mesh->HasBones())
    {
      ExtractBoneWeightForVertices(mesh, scene, model_data, range.first_vertex);
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

  auto AssimpModelLoaderImpl::SetVertexBoneDataToDefault(Vertex& vertex) noexcept -> void
  {
    for (int i =0; i < modelmanager::detail::kMaxBoneInfluence; ++i)
    {
      vertex.bone_ids[i] = -1;
      vertex.weights[i]  = 0.f;
    }
  }

  auto AssimpModelLoaderImpl::SetVertexBoneData(Vertex& vertex, int bone_id, float weight) noexcept -> void
  {
    for (int i = 0; i < modelmanager::detail::kMaxBoneInfluence; ++i)
    {
      if (vertex.bone_ids[i] < 0)
      {
        vertex.weights[i] = weight;
        vertex.bone_ids[i] = bone_id;
        break;
      }
    }
  }

  auto AssimpModelLoaderImpl::ExtractBoneWeightForVertices(aiMesh* mesh, const aiScene* scene, CpuModelData& model_data, std::size_t mesh_first_vertex) noexcept -> void
  {
    auto& vertices = model_data.vertices;
    auto& bone_counter = model_data.bone_counter;
    auto& bone_info = model_data.bone_info_map;

    for (int bone_index = 0; bone_index < mesh->mNumBones; ++bone_index)
    {
      int bone_id = -1;
      std::string bone_name(mesh->mBones[bone_index]->mName.C_Str());
      if (bone_info.find(bone_name) == bone_info.end())
      {
        BoneInfo new_bone_info;
        new_bone_info.id = bone_counter;
        new_bone_info.offset = ConvertMatrixToGLMFormat(mesh->mBones[bone_index]->mOffsetMatrix);
        bone_info.try_emplace(bone_name, new_bone_info);
        bone_id = bone_counter++;
      }
      else 
      {
        bone_id = bone_info[bone_name].id;
      }
      assert(bone_id != -1);
      auto weights = mesh->mBones[bone_index]->mWeights;
      int num_weights = mesh->mBones[bone_index]->mNumWeights;

      for (int weight_index{}; weight_index < num_weights; ++weight_index)
      {
        int vertex_id = mesh_first_vertex + weights[weight_index].mVertexId;
        float weight = weights[weight_index].mWeight;
        assert(vertex_id < vertices.size());
        SetVertexBoneData(vertices[vertex_id], bone_id, weight);
      }
    }
  }

    auto AssimpModelLoaderImpl::ReadHeirarchyData(animation::AssimpNodeData& dest, const aiNode* src) -> void
    {
      assert(src);

      dest.name = src->mName.data;
      dest.transformation = ConvertMatrixToGLMFormat(src->mTransformation);
      dest.children_count = src->mNumChildren;

      dest.children.reserve(dest.children_count);
      for (int i{}; i < src->mNumChildren; ++i)
      {
        animation::AssimpNodeData new_data;
        ReadHeirarchyData(new_data, src->mChildren[i]);
        dest.children.push_back(new_data);
      }
    }

    auto AssimpModelLoaderImpl::ReadMissingBones(const aiAnimation* animation, CpuModelData& model_data) -> void
    {
      int size = animation->mNumChannels;

      auto& bone_info_map = model_data.bone_info_map;
      int& bone_count = model_data.bone_counter;

      for (int i{}; i < size; ++i)
      {
        auto channel = animation->mChannels[i];
        std::string bone_name = channel->mNodeName.data;

        if (bone_info_map.find(bone_name) == bone_info_map.end())
        {
          bone_info_map[bone_name].id = bone_count;
          bone_count++;
        }
        model_data.bones.emplace_back(channel->mNodeName.data, bone_info_map[bone_name].id, channel);
      }
    }

  std::unique_ptr<AssimpModelLoaderImpl> MakeAssimpModelLoader() noexcept
  {
    return std::make_unique<AssimpModelLoaderImpl>();
  }

  auto AssimpModelLoaderImpl::ConvertMatrixToGLMFormat(const aiMatrix4x4& from) -> glm::mat4
  {
    glm::mat4 to;
    // the a,b,c,d in assimp is the row; the 1,2,3,4 is the column
    to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
    to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
    to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
		to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
		return to;
  }


} // namespace avion::core::assimp::detail
