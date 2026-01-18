#pragma once

#include "glad/glad.h"
#include "glfw/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace avion::gfx {

    // Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
    enum class CameraMovement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    // Default camera values
    static const float YAW         = -90.0f;
    static const float PITCH       =  0.0f;
    static const float SPEED       =  2.5f;
    static const float SENSITIVITY =  0.1f;
    static const float ZOOM        =  45.0f;

    // An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
    class Camera
    {
    public:
        // constructor with vectors
        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
        // constructor with scalar values
        Camera(float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z, float yaw, float pitch);

        // returns the view matrix calculated using Euler Angles and the LookAt Matrix
        glm::mat4 GetViewMatrix();

        // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
        void ProcessKeyboard(CameraMovement direction, float delta_time);

        // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
        void ProcessMouseMovement(float x_offset, float yoffset, GLboolean constrain_pitch = true);

        // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
        void ProcessMouseScroll(float y_offset);

    private:
        // camera Attributes
        glm::vec3 camera_position_;
        glm::vec3 camera_front_;
        glm::vec3 camera_up_;
        glm::vec3 camera_right_;
        glm::vec3 camera_world_up_;

        // euler Angles
        float angle_yaw_;
        float angle_pitch_;

        // camera options
        float movement_speed_;
        float mouse_senstivity_;
        float zoom_;

        // calculates the front vector from the Camera's (updated) Euler Angles
        void UpdateCameraVectors();
    };

} // namespace avion::gfx