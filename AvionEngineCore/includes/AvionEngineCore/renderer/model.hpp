#ifndef AVION_GFX_MODEL_H 
#define AVION_GFX_MODEL_H
  
  #include <memory>
  #include <vector>
  #include <span>

  #include "AvionEngineCore/macro.h"
  #include "AvionEngineCore/core/material.hpp"
  #include "AvionEngineCore/core/ModelManager/ModelData.hpp"
  #include "AvionEngineCore/core/TextureManager/TextureManager.hpp"
  #include "AvionEngineCore/renderer/transform.hpp"

  namespace avion::core::resman
  {
    class ResourceManager;
  }

  namespace avion::gfx
  {
    class Model 
    {
      public:
        using ResManager      = core::resman::ResourceManager;
        using ModelData       = core::modelmanager::detail::ModelData;
        using Transform       = gfx::Transform;
        using TextureHandler  = core::texturemanager::TextureManager::TextureHandler;
        using MeshRange       = std::span<core::modelmanager::detail::MeshRange>;
        using Material        = core::material::Material;
        using Color           = glm::vec3;

        Model() = delete;
        Model(const std::string& filename, const ModelData& model_data, const Material& material);
        Model(const std::string& filename, const Model& other);
        Model(const Model& other);
        Model(Model&& other);

        Model& operator=(const Model& other);
        Model& operator=(Model&& other) noexcept;

        std::string GetFileName() const noexcept;
        Transform&  GetTransform() noexcept;
        ModelData&  GetModelData() noexcept;
        MeshRange   GetMeshRange() noexcept;
        Material&   GetMaterial() noexcept;

        const Material&   GetMaterial() const noexcept;
        const Transform&  GetTransform() const noexcept;
        const ModelData&  GetModelData() const noexcept;

        void Swap(Model& other) noexcept;

        ~Model() = default;
      private:
     
      private:
        std::string m_filename;
        ModelData m_data;
        Transform m_transform;  
        Material m_material;
                 
        // Material       m_material;        
    };
    
    void swap(Model& lhs, Model& rhs) noexcept;
  } // namespace avion::gfx

#endif
