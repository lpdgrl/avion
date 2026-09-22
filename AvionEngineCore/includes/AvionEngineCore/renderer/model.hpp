#ifndef AVION_GFX_MODEL_H
#define AVION_GFX_MODEL_H

  #include <span>
  #include <string_view>

  #include "AvionEngineCore/macro.h"
  #include "AvionEngineCore/core/material.hpp"
  #include "AvionEngineCore/core/ModelManager/ModelData.hpp"
  #include "AvionEngineCore/core/TextureManager/TextureManager.hpp"
  #include "AvionEngineCore/renderer/transform.hpp"
  #include "AvionEngineCore/core/Animation/Animator.hpp"
  #include "AvionEngineCore/core/Animation/Animation.hpp"

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
        using CpuModelData    = core::modelmanager::detail::CpuModelData;
        using Transform       = gfx::Transform;
        using TextureHandler  = core::texturemanager::TextureManager::TextureHandler;
        using MeshRange       = std::span<core::modelmanager::detail::MeshRange>;
        using Material        = core::material::Material;
        using Color           = glm::vec3;
        using Animator        = core::animation::Animator;
        using Animation       = core::animation::Animation;
        using AnimationId     = core::animation::AnimationId;

        Model() = delete;
        Model(std::string_view filename, CpuModelData& cpu_model_data, const Material& material, bool has_animation, std::string_view extension = {});
        Model(std::string_view filename, const Model& other);
        Model(const Model& other);
        Model(Model&& other);

        Model& operator=(const Model& other);
        Model& operator=(Model&& other) noexcept;

        std::string   GetFileName() const noexcept;
        auto          GetFileExtension() const noexcept -> std::string;
        Transform&    GetTransform() noexcept;
        CpuModelData& GetCpuModelData() noexcept;
        MeshRange     GetMeshRange() noexcept;
        Material&     GetMaterial() noexcept;
        auto          GetAnimator() noexcept -> Animator&;

        auto HasAnimation() const noexcept -> bool;
        const Material&   GetMaterial() const noexcept;
        const Transform&  GetTransform() const noexcept;
        const CpuModelData&  GetCpuModelData() const noexcept;

        void Swap(Model& other) noexcept;

        ~Model() = default;

      private:

      private:
        // std::unordered_map<int, Animation> m_animation_set;
        std::string m_filename;
        std::string m_file_extension;
        CpuModelData m_cpu_model_data;
        Transform m_transform;
        Material m_material;
        Animator m_animator;
        bool m_has_animation{};
    };

    void swap(Model& lhs, Model& rhs) noexcept;
  } // namespace avion::gfx

#endif
