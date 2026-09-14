#ifndef AVION_CORE_ANIMATION_ANIMATION_H
#define AVION_CORE_ANIMATION_ANIMATION_H 1

  #include <unordered_map>
  #include <algorithm>
  #include <ranges>

  #include "AvionEngineCore/core/Animation/Bone.hpp"
  #include "AvionEngineCore/core/ModelManager/ModelData.hpp"

  namespace avion::core::animation
  {
    enum class AnimationId : int
    {
      Unknown = -1,
      Idle  = 0,
      Walk  = 1,
      Dance = 2,
      Run   = 3,
    };

    class Animation
    {
      public:
        using BoneInfo = modelmanager::detail::BoneInfo;
        using BonesInfo = std::unordered_map<std::string, BoneInfo>;

        Animation() = delete;
        Animation(std::vector<Bone>& bones, AssimpNodeData& root_node, BonesInfo& bones_info, float duration, int ticks_per_second, AnimationId animation_id);

        Animation(const Animation& other) = delete;
        Animation(Animation&& other) = delete;
        Animation& operator=(const Animation& rhs) = delete;
        Animation& operator=(Animation& rhs) = delete;

        ~Animation() = default;

        auto FindBone(const std::string& name) -> Bone*;
        auto GetTicksPerSceond() const noexcept -> float;
        auto GetDuration() const noexcept -> float;
        auto GetRootNode() const noexcept -> const AssimpNodeData&;
        auto GetBoneIDMap() const noexcept -> const BonesInfo&;
        auto GetBones() const noexcept -> const std::vector<Bone>&;
        auto GetId() const noexcept -> AnimationId;

      protected:
      
      private:
        AssimpNodeData m_root_node;
        BonesInfo m_bones_info;
        std::vector<Bone> m_bones;
        float m_duration{};
        int m_ticks_per_second{};
        AnimationId m_id{-1};
    };
  } // namespace avion::core::animation

#endif