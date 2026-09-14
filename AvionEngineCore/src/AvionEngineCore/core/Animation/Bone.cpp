#include "AvionEngineCore/core/Animation/Bone.hpp"

  namespace avion::core::animation
  {
    Bone::Bone(const std::string& name, int ID, const aiNodeAnim* channel)
    : m_num_positions(channel->mNumPositionKeys)
    , m_num_rotations(channel->mNumRotationKeys)
    , m_num_scalings(channel->mNumScalingKeys)
    , m_local_transform(1.f)  
    , m_name(name)
    , m_id(ID)
    {
      for (int position_index{}; position_index < m_num_positions; ++position_index)
      {
        aiVector3D ai_position = channel->mPositionKeys[position_index].mValue;
        float time_stamp = channel->mPositionKeys[position_index].mTime;
        KeyPosition data
        {
          .position = glm::vec3(ai_position.x, ai_position.y, ai_position.z),
          .time_stamp = time_stamp
        };
        m_positions.emplace_back(std::move(data));
      }

      for (int rotation_index{}; rotation_index < m_num_rotations; ++rotation_index)
      {
        aiQuaternion aiOrientation = channel->mRotationKeys[rotation_index].mValue;
        float time_stamp = channel->mRotationKeys[rotation_index].mTime;
        KeyRotation data
        {
          data.orientation = glm::quat(aiOrientation.w, aiOrientation.x, aiOrientation.y, aiOrientation.z),
          data.time_stamp = time_stamp
        };
        m_rotations.emplace_back(std::move(data));
      }

      for (int scaling_index{}; scaling_index < m_num_scalings; ++scaling_index)
      {
        aiVector3D ai_scale = channel->mScalingKeys[scaling_index].mValue;
        float time_stamp = channel->mScalingKeys[scaling_index].mTime;
        KeyScale data
        {
          data.scale = glm::vec3(ai_scale.x, ai_scale.y, ai_scale.z),
          data.time_stamp = time_stamp
        };
        m_scales.emplace_back(std::move(data));
      }
    }

    auto Bone::Update(float animation_time) noexcept -> void
    {
      glm::mat4 translation = InterpolatePosition(animation_time);
      glm::mat4 rotation = InterpolateRotation(animation_time);
      glm::mat4 scale = InterpolateScaling(animation_time);
      m_local_transform = translation * rotation * scale;
    }

    auto Bone::GetLocalTransform() const noexcept -> glm::mat4
    {
      return m_local_transform;
    }

    auto Bone::GetBoneName() const noexcept -> const std::string&
    {
      return m_name;
    }

    auto Bone::GetBoneId() const noexcept -> int
    {
      return m_id;
    }

    auto Bone::GetPositionIndex(float animation_time) const noexcept -> int
    {
      for (int index{}; index < m_num_positions - 1; ++index)
      {
        if (animation_time < m_positions[index + 1].time_stamp)
        {
          return index;
        }
      }
      assert(false);
    }

    auto Bone::GetRotationIndex(float animation_time) const noexcept -> int
    {
      int index{};
      for (; index < m_num_rotations - 1; ++index)
      {
        if (animation_time < m_rotations[index + 1].time_stamp)
        {
          return index;
        }
      }
      AV_LOG_ERROR("Bone::GetRotationIndex: index " + std::to_string(index));
      // assert(false);
      return index - 1;
    }

    auto Bone::GetScaleIndex(float animation_time) const noexcept -> int
    {
      for (int index{}; index < m_num_scalings - 1; ++index)
      {
        if (animation_time < m_scales[index + 1].time_stamp)
        {
          return index;
        }
      }
      assert(false);
    }

    auto Bone::GetScaleFactor(float last_time_stamp, float next_time_stamp, float animation_time)
      const noexcept -> float
    {
      float scale_factor{};
      float mid_way_length = animation_time - last_time_stamp;
      float frames_diff = next_time_stamp - last_time_stamp;
      scale_factor = mid_way_length / frames_diff;
      return scale_factor;
    }

    auto Bone::InterpolatePosition(float animation_time) const noexcept -> glm::mat4
    {
      if (1 == m_num_positions)
      {
        return glm::translate(glm::mat4(1.0f), m_positions[0].position);
      }
      int p0_index = GetPositionIndex(animation_time);
      int p1_index = p0_index + 1;
      float scale_factor = GetScaleFactor(m_positions[p0_index].time_stamp, m_positions[p1_index].time_stamp, animation_time);
      glm::vec3 final_position = glm::mix(m_positions[p0_index].position, m_positions[p1_index].position, scale_factor);
      return glm::translate(glm::mat4(1.0f), final_position);
    }

    auto Bone::InterpolateRotation(float animation_time) const noexcept -> glm::mat4
    {
      if (1 == m_num_rotations)
      {
        auto rotation = glm::normalize(m_rotations[0].orientation);
        return glm::toMat4(rotation);
      }

      int p0_index = GetRotationIndex(animation_time);
      int p1_index = p0_index + 1;
      float scale_factor = GetScaleFactor(m_rotations[p0_index].time_stamp, m_rotations[p1_index].time_stamp, animation_time);
      glm::quat final_rotation = glm::slerp(m_rotations[p0_index].orientation, m_rotations[p1_index].orientation, scale_factor);
      final_rotation = glm::normalize(final_rotation);
      return glm::toMat4(final_rotation);
    }

    auto Bone::InterpolateScaling(float animation_time) const noexcept -> glm::mat4
    {
      if (1 == m_num_scalings)
      {
        return glm::scale(glm::mat4(1.0f), m_scales[0].scale);
      }

      int p0_index = GetScaleIndex(animation_time);
      int p1_index = p0_index + 1;
      float scale_factor = GetScaleFactor(m_scales[p0_index].time_stamp, m_scales[p1_index].time_stamp, animation_time);
      glm::vec3 final_scale = glm::mix(m_scales[p0_index].scale, m_scales[p1_index].scale, scale_factor);
      return glm::scale(glm::mat4(1.0f), final_scale);
    } 
  } // namespace avion::core::animation