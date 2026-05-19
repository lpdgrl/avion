#ifndef AVION_CORE_GFX_TRANSFORM_H
#define AVION_CORE_GFX_TRANSFORM_H

  #include "glm/glm.hpp"
  #include "glm/gtc/matrix_transform.hpp"

  namespace avion::gfx
  {
    enum class AxisRotate {
        NONE   = -1,
        AXIS_X =  0,
        AXIS_Y =  1,
        AXIS_Z =  2,
    };

    struct Transform {
      glm::vec3 position{0.f}; 
      glm::vec3 rotation{0.f};
      glm::vec3 size{1.f};

      float value_rotate = 0.f;

      AxisRotate axis = AxisRotate::NONE;

      inline glm::mat4 GetMatrix() const
      {
        glm::mat4 matrix{1.f};

        matrix = glm::translate(matrix, position);
        float pi = 3.14159265f;
        matrix = glm::rotate(matrix, glm::radians(rotation.x * pi), glm::vec3(0.f, 1.f, 0.f));
        matrix = glm::rotate(matrix, glm::radians(rotation.y * pi), glm::vec3(1.f, 0.f, 0.f));
        matrix = glm::rotate(matrix, glm::radians(rotation.z * pi), glm::vec3(0.f, 0.f, 1.f));

        matrix = glm::scale(matrix, size);

        return matrix;
      }
    };
  } // namespace avion::gfx

#endif