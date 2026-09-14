#ifndef AVION_CORE_ANIMATION_ANIMATOR_H
#define AVION_CORE_ANIMATION_ANIMATOR_H 1

  #include <vector>
  #include <cmath>

  #include "AvionEngineCore/core/ModelManager/ModelData.hpp"

  namespace avion::core::animation
  {
    class Animator
    {
      public:
        using CpuModelData = core::modelmanager::detail::CpuModelData;
        using AssimpNodeData = core::animation::AssimpNodeData;

        Animator() = delete;
        Animator(CpuModelData& cpu_model_data);

        auto UpdateAnimation(float dt) -> void;
        auto PlayAnimation(CpuModelData& cpu_model_data) -> void;
        auto CalculateBoneTransform(const AssimpNodeData& node, glm::mat4 parent_transformatin) -> void;

        auto GetFinalBoneMatrices() 
          -> std::vector<glm::mat4>;

        ~Animator() = default;
        
      protected:

      private:
        CpuModelData& m_cpu_model_data;
        std::vector<glm::mat4> m_final_bone_matrices;
        float m_current_time{};
        float m_delta_time{};
    };
  } // namespace avion::animation

#endif