#ifndef AVION_CORE_CAMERA_PROXY_H
#define AVION_CORE_CAMERA_PROXY_H 1

  namespace avion::gfx
  {
    class Camera;
  }

  namespace avion::core::common
  {

    struct CameraInfo
    {
      double position_x{};
      double position_y{};
      double position_z{};

      double front_position_x{};
      double front_position_y{};
      double front_position_z{};

      double up_position_x{};
      double up_position_y{};
      double up_position_z{};

      double right_position_x{};
      double right_position_y{};
      double right_position_z{};
      
      double world_up_position_x{};
      double world_up_position_y{};
      double world_up_position_z{};

      float angle_yaw{};
      float angle_pitch{};

      float movement_speed{};
      float mouse_senstivity{};
      float zoom{};
    };

    class CameraProxy
    {
      public:


        CameraProxy() = delete;
        CameraProxy(gfx::Camera& camera);

        CameraProxy(const CameraProxy&) = delete("Copy ctor disable");
        CameraProxy(CameraProxy&&) = delete("Move ctor disable");

        CameraProxy& operator=(const CameraProxy&) = delete("Copy assign disable");
        CameraProxy& operator=(CameraProxy&&) = delete("Move assign disable");

        ~CameraProxy() = default;

        void Update(int action, double delta_time) noexcept;
        void Update(double xoffset, double yoffset) noexcept;

        auto GetCameraInfo() const noexcept -> CameraInfo;

      protected:

      private:
        gfx::Camera& m_camera;
    };
  } // namespace avion::core::common

#endif