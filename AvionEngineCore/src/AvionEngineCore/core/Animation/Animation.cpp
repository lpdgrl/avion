#include "AvionEngineCore/core/Animation/Animation.hpp"

namespace avion::core::animation
{
  Animation::Animation(std::vector<Bone>& bones, AssimpNodeData& root_node, BonesInfo& bones_info, float duration, int ticks_per_second, AnimationId animation_id)
  : m_root_node(std::move(root_node))
  , m_bones_info(std::move(bones_info))
  , m_bones(std::move(bones))
  , m_duration(duration)
  , m_ticks_per_second(ticks_per_second)
  , m_id(animation_id)
  {
    
  }

  auto Animation::FindBone(const std::string& name) -> Bone*
  {
    Bone *bone{};
    
    if (auto it_bone = std::ranges::find_if(m_bones, [&name](const Bone& bone)
    {
      return bone.GetBoneName() == name;
    });
      it_bone != m_bones.end())
    {
      bone = &(*it_bone);
    }

    return bone;
  }

  auto Animation::GetTicksPerSceond() const noexcept -> float
  {
    return m_ticks_per_second;
  }

  auto Animation::GetDuration() const noexcept -> float
  {
    return m_duration;
  }

  auto Animation::GetRootNode() const noexcept -> const AssimpNodeData&
  {
    return m_root_node;
  }

  auto Animation::GetBoneIDMap() const noexcept -> const BonesInfo&
  {
    return m_bones_info;
  }

  auto Animation::GetBones() const noexcept -> const std::vector<Bone>&
  {
    return m_bones;
  }

  auto Animation::GetId() const noexcept -> AnimationId
  {
    return m_id;
  }

} // namespace avion::core::animation
