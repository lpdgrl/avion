#ifndef AVION_GFX_MODEL_H 
#define AVION_GFX_MODEL_H
  
  #include <memory>
  #include <vector>
  #include <span>

  #include "AvionEngineCore/macro.h"

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
        using ResManager = core::resman::ResourceManager;
        using ModelData = core::modelmanager::detail::ModelData;
        using MeshRange = std::span<core::modelmanager::detail::MeshRange>;
        using Transform = gfx::Transform;
        using TextureHandler = core::texturemanager::TextureManager::TextureHandler;
        using Material = std::vector<TextureHandler>;
        using Color = glm::vec3;

        Model() = delete;
        Model(const std::string& filename, const ModelData& model_data, const Material& material);
        Model(const std::string& filename, const ModelData& model_data, Color color);
        Model(const std::string& filename, const Model& other);
        Model(const Model& other);
        Model(Model&& other);

        Model& operator=(const Model& other);
        Model& operator=(Model&& other) noexcept;

        std::string GetFileName() const noexcept;

        Transform&  GetTransform() noexcept;
        const Transform& GetTransform() const noexcept;

        ModelData& GetModelData() noexcept;
        const ModelData& GetModelData() const noexcept;

        MeshRange GetMeshRange() noexcept;

        Material& GetMaterial() noexcept;
        const Material& GetMaterial() const noexcept;

        Color& GetColor() noexcept;
        Color GetColor() const noexcept;
        
        void Swap(Model& other) noexcept;

        ~Model() = default;
      private:
     
      private:
        std::string m_filename;
        ModelData m_data;
        Transform m_transform;  
        Material m_material;
        Color m_solid_color;
                 
        // Material       m_material;        
    };
    
    void swap(Model& lhs, Model& rhs) noexcept;
  } // namespace avion::gfx

#endif
