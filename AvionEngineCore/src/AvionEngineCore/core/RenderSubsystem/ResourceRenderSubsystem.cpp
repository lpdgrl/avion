#include "AvionEngineCore/core/RenderSubsystem/ResourceRenderSubsystem.hpp"

#include "AvionEngineCore/api/backend/backend.hpp"

#include "AvionEngineCore/core/ModelManager/ModelManager.hpp"
#include "AvionEngineCore/core/ModelManager/ModelData.hpp"

#include "AvionEngineCore/core/TextureManager/TextureManager.hpp"
#include "AvionEngineCore/core/material.hpp"

#include "AvionEngineCore/core/resource_manager.hpp"

namespace avion::core::rendersubsystem
{
  auto ResourceRenderSubsystem::PrepareResource(std::string_view path_to_resource, 
    api::backend::Backend& backend, 
    mdm::ModelManager& mm, 
    txm::TextureManager& tm) -> std::uint64_t
  {
    auto cpu_model_data = LoadCpuModelData(path_to_resource, mm);
    const detail::GpuResource gpu_resource = backend.CreateModel(cpu_model_data).id;
    auto material = LoadModelTextures(path_to_resource, cpu_model_data.texture_source, tm, backend);
    const auto model_id = mm.CreateModel(path_to_resource, cpu_model_data, material);

    m_gpu_resource_storage.try_emplace(model_id, gpu_resource);
    return model_id;
  }

  auto ResourceRenderSubsystem::Contains(detail::ModelId model_id) const noexcept -> bool
  {
    if (m_gpu_resource_storage.find(model_id) == m_gpu_resource_storage.cend())
    {
      return false;
    }
    return true;
  }

  auto ResourceRenderSubsystem::Get(detail::ModelId model_id) const noexcept -> detail::GpuResource
  {
    detail::GpuResource gpu_resource = m_gpu_resource_storage.at(model_id);
    return gpu_resource;
  }

  auto ResourceRenderSubsystem::Remove(detail::ModelId model_id) -> bool
  {
    if (Contains(model_id))
    {
      m_gpu_resource_storage.erase(model_id);
      return true;
    }
    return false;
  }

  auto ResourceRenderSubsystem::LoadCpuModelData(std::string_view path_to_model, modelmanager::ModelManager& mm) -> modelmanager::detail::CpuModelData
  {
    modelmanager::detail::CpuModelData model_data;
    
    if (auto result = mm.Load(path_to_model))
    {
      model_data = std::move(result.value());
    }
    return model_data;
  }
  
  auto ResourceRenderSubsystem::LoadModelTextures(std::string_view parent_path, 
    std::vector<mdm::detail::TextureSource>& textures, 
    txm::TextureManager& tm, 
    api::backend::Backend& backend) -> material::Material
  {
    using namespace material;
    auto path_parent = resman::ResourceManager::FsPath(parent_path).parent_path();

    Material material;
    material.type = (textures.empty()) ? MaterialType::kRegular : MaterialType::kTexture;

    for (const auto& texture : textures)
    {
      if (auto result = tm.LoadTexture(path_parent / texture.path))
      {
        const auto texture_raw_data = result.value();
        auto [texture_type, gpu_resource] = backend.CreateTexture2D(texture_raw_data);
        switch (texture_type)
        {
          case txm::detail::TextureType::kDiffuse:
          {
            material.diffuse_texture.emplace_back(texture_type, gpu_resource);
            break;
          }
          case txm::detail::TextureType::kSpecular:
          {
            material.specular_texture.emplace_back(texture_type, gpu_resource);
            break;
          }
        }
      }
    }
    return material;
  }
  
} // namespace avion::core::rendersystem