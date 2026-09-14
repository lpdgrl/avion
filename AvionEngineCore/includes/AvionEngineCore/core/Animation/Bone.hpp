#ifndef AVION_CORE_ANIMATION_BONE_H
#define AVION_CORE_ANIMATION_BONE_H 1

  #include <vector>
  #include <string>

  #include <assimp/scene.h>

  #include "glm/glm.hpp"
  #include "glm/gtc/matrix_transform.hpp"
  #include "glm/gtx/quaternion.hpp"

  #include "AvionEngineCore/macro.h"

  namespace avion::core::animation
  {
    struct AssimpNodeData
    {
      glm::mat4 transformation;
      std::string name;
      int children_count{};
      std::vector<AssimpNodeData> children;
    };

    struct KeyPosition
    {
      glm::vec3 position{};
      float time_stamp{};
    };

    struct KeyRotation
    {
      glm::quat orientation{};
      float time_stamp{};
    };

    struct KeyScale
    {
      glm::vec3 scale;
      float time_stamp;
    };

    class Bone
    {
      public:
        Bone(const std::string& name, int ID, const aiNodeAnim* channel);

        auto Update(float animation_time) noexcept -> void;
        auto GetLocalTransform() const noexcept -> glm::mat4;
        // TODO: Replace return value from std::string to string_view
        auto GetBoneName() const noexcept -> const std::string&;
        auto GetBoneId() const noexcept -> int;
        auto GetPositionIndex(float animation_time) const noexcept -> int;
        auto GetRotationIndex(float animation_time) const noexcept -> int;
        auto GetScaleIndex(float animation_time) const noexcept -> int;

      protected:

      private:
        auto GetScaleFactor(float last_time_stamp, float next_time_stamp, float animation_time)
          const noexcept -> float;

        auto InterpolatePosition(float animation_time) const noexcept -> glm::mat4;
        auto InterpolateRotation(float animation_time) const noexcept -> glm::mat4;
        auto InterpolateScaling(float animation_time) const noexcept -> glm::mat4; 

      private:
        std::vector<KeyPosition> m_positions;
        std::vector<KeyRotation> m_rotations;
        std::vector<KeyScale> m_scales;
        int m_num_positions{};
        int m_num_rotations{};
        int m_num_scalings{};

        glm::mat4 m_local_transform;
        std::string m_name;
        int m_id;
    };
  } // namespace avion::core::animation


#endif