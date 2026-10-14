#ifndef AVION_CORE_RENDER_SUBSYSTEM_RESOURCE_H
#define AVION_CORE_RENDER_SUBSYSTEM_RESOURCE_H 1

#include <unordered_map>
#include <cstdint>
#include <string_view>
#include <vector>

#include "AvionEngineCore/macro.h"

// forward declaration
namespace avion::core
{ 
  namespace modelmanager
  {
    namespace detail
    {
      struct CpuModelData;
      struct TextureSource;
    }
    class ModelManager;
  }
  namespace texturemanager
  {
    class TextureManager;
  }
  namespace material
  {
    struct Material;
  }
}

// forward declaration
namespace avion::api::backend
{
  class Backend;
}

namespace avion::core::rendersubsystem
{
  namespace detail
  {
    using GpuResource = std::uint64_t;
    using ModelId     = std::uint64_t;
  } // namespace avion::core::rendersubsystem::detail

  namespace mdm = modelmanager;
  namespace txm = texturemanager;
  
  class ResourceRenderSubsystem
  {
    public:
      ResourceRenderSubsystem() = default;

      ResourceRenderSubsystem(const ResourceRenderSubsystem& other) = delete;
      ResourceRenderSubsystem(ResourceRenderSubsystem&& other) = delete;
      ResourceRenderSubsystem& operator=(const ResourceRenderSubsystem& rhs) = delete;
      ResourceRenderSubsystem& operator=(ResourceRenderSubsystem&& rhs) = delete;

      ~ResourceRenderSubsystem() = default;

      AV_ATTR_NODISCARD
      auto PrepareResource(std::string_view path_to_resource, 
        api::backend::Backend& backend, 
        mdm::ModelManager& mm, 
        txm::TextureManager& tm) -> std::uint64_t;

      AV_ATTR_NODISCARD
      auto Get(detail::ModelId model_id) const noexcept -> detail::GpuResource;
      
      AV_ATTR_NODISCARD
      auto Contains(detail::ModelId model_id) const noexcept -> bool;

      // Delete gpu resource!!
      AV_ATTR_NODISCARD
      auto Remove(detail::ModelId model_id) -> bool;

    private:
      auto LoadCpuModelData(std::string_view path_to_model, 
        mdm::ModelManager& mm) -> mdm::detail::CpuModelData;
      auto LoadModelTextures(std::string_view parent_path, 
        std::vector<mdm::detail::TextureSource>& textures, 
        txm::TextureManager& tm, 
        api::backend::Backend& backend) -> material::Material;

    private:
      std::unordered_map<std::uint64_t, std::uint64_t> m_gpu_resource_storage;
  };

} // namespace avion::core::rendersystem

#endif