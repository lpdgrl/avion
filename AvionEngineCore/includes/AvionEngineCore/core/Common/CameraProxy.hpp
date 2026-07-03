#ifndef AVION_CORE_CAMERA_PROXY_H
#define AVION_CORE_CAMERA_PROXY_H 1

  namespace avion::gfx
  {
    class Camera;
  }

  namespace avion::core::common
  {
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

      protected:

      private:
        gfx::Camera& m_camera;
    };
  } // namespace avion::core::common

#endif