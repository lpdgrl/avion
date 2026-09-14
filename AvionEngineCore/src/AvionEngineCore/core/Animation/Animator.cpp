#include "AvionEngineCore/core/Animation/Animator.hpp"

namespace avion::core::animation
{
  Animator::Animator(CpuModelData& cpu_model_data)
  : m_cpu_model_data(cpu_model_data)
  {
    m_final_bone_matrices.reserve(300);

    for (int i = 0; i < 300; ++i)
    {
      m_final_bone_matrices.push_back(glm::mat4(1.0f));
    }
  }

  auto Animator::UpdateAnimation(float dt) -> void
  {
    m_delta_time = dt;
    m_current_time += m_cpu_model_data.ticks_per_second * dt;
    m_current_time = std::fmod(m_current_time, m_cpu_model_data.duration);
    CalculateBoneTransform(m_cpu_model_data.root_node, glm::mat4(1.f));
  }

  auto Animator::PlayAnimation(CpuModelData& cpu_model_data) -> void
  {
    m_cpu_model_data = cpu_model_data;
    m_current_time = 0.f;
  }

  auto Animator::CalculateBoneTransform(const AssimpNodeData& node, glm::mat4 parent_transformatin) 
          -> void
  {
    std::string node_name(node.name);
    glm::mat4 node_transform = node.transformation;

    Bone* bone{};
    if (auto it_bone = std::ranges::find_if(m_cpu_model_data.bones, [&node_name](const Bone& bone)
    {
      return bone.GetBoneName() == node_name;
    });
      it_bone != m_cpu_model_data.bones.end())
    {
      bone = &(*it_bone);
    }
    
    if (bone)
    {
      bone->Update(m_current_time);
      node_transform = bone->GetLocalTransform();
    }

    glm::mat4 global_transform = parent_transformatin * node_transform;

    if (auto it = m_cpu_model_data.bone_info_map.find(node_name); it != m_cpu_model_data.bone_info_map.end())
    {
      int index = it->second.id;
      glm::mat4 offset = it->second.offset;
      m_final_bone_matrices[index] = global_transform * offset;
    }

    for (int i{}; i < node.children_count; ++i)
    {
      CalculateBoneTransform(node.children[i], global_transform);
    }
  }

  auto Animator::GetFinalBoneMatrices() -> std::vector<glm::mat4>
  {
    return m_final_bone_matrices;
  }
        
} // namespace avion::core::animation