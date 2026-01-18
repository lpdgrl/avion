#include "../../../includes/AvionEngineCore/renderer/camera.hpp"

namespace avion::gfx {

    // constructor with vectors
    Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
            : camera_position_(position)
            , camera_front_(glm::vec3(0.0f, 0.0f, -1.0f))
            , camera_up_(0)
            , camera_right_(0)
            , camera_world_up_(up)
            , angle_yaw_(yaw)
            , angle_pitch_(pitch)
            , movement_speed_(SPEED)
            , mouse_senstivity_(SENSITIVITY)
            , zoom_(ZOOM) 
    {
        UpdateCameraVectors();
    }

    // constructor with scalar values
    Camera::Camera(float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z, float yaw, float pitch) 
            : camera_position_(glm::vec3(pos_x, pos_y, pos_z))
            , camera_front_(glm::vec3(0.0f, 0.0f, -1.0f))
            , camera_up_(0)
            , camera_right_(0)
            , camera_world_up_(glm::vec3(up_x, up_y, up_z))
            , angle_yaw_(yaw)
            , angle_pitch_(pitch)
            , movement_speed_(SPEED)
            , mouse_senstivity_(SENSITIVITY)
            , zoom_(ZOOM)
    {
        UpdateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 Camera::GetViewMatrix() {
        return glm::lookAt(camera_position_, camera_position_ + camera_front_, camera_up_);
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void Camera::ProcessKeyboard(CameraMovement direction, float delta_time) {
        float velocity = movement_speed_ * delta_time;

        switch (direction) {
            case CameraMovement::FORWARD: 
                camera_position_ += camera_front_ * velocity;
                break;
            case CameraMovement::BACKWARD:
                camera_position_ -= camera_front_ * velocity;
                break;
            case CameraMovement::LEFT:
                camera_position_ -= camera_right_ * velocity;
                break;
            case CameraMovement::RIGHT:
                camera_position_ += camera_right_ * velocity;
                break;
        }
        // camera_position_.y = 0.0f;
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void Camera::ProcessMouseMovement(float x_offset, float y_offset, GLboolean constrain_pitch) {
        x_offset *= mouse_senstivity_;
        y_offset *= mouse_senstivity_;

        angle_yaw_   += x_offset;
        angle_pitch_ += y_offset;
        
        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrain_pitch)
        {
            if (angle_pitch_ > 89.0f)
                angle_pitch_ = 89.0f;
            if (angle_pitch_ < -89.0f)
                angle_pitch_ = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        UpdateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void Camera::ProcessMouseScroll(float y_offset) {
        zoom_ -= static_cast<float>(y_offset);

        if (zoom_ < 1.0f)
            zoom_ = 1.0f;
        if (zoom_ > 45.0f)
            zoom_ = 45.0f;
    }

    // calculates the front vector from the Camera's (updated) Euler Angles
    void Camera::UpdateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;

        front.x = cos(glm::radians(angle_yaw_)) * cos(glm::radians(angle_pitch_));
        front.y = sin(glm::radians(angle_pitch_));
        front.z = sin(glm::radians(angle_yaw_)) * cos(glm::radians(angle_pitch_));

        camera_front_ = glm::normalize(front);

        // also re-calculate the Right and Up vector
        camera_right_ = glm::normalize(glm::cross(camera_front_, camera_world_up_));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        camera_up_    = glm::normalize(glm::cross(camera_right_, camera_front_));
    }

} // namespace avion::gfx