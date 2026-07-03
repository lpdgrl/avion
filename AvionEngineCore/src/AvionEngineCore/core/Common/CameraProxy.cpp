#include "AvionEngineCore/core/Common/CameraProxy.hpp"
#include "AvionEngineCore/renderer/camera.hpp"

  namespace avion::core::common
  {

    CameraProxy::CameraProxy(gfx::Camera& camera): m_camera(camera) 
    {

    }

    void CameraProxy::Update(int action, double delta_time) noexcept
    {
      using namespace gfx;
      m_camera.ProcessKeyboard(static_cast<CameraMovement>(action), delta_time);
    }

    void CameraProxy::Update(double xoffset, double yoffset) noexcept
    {
      m_camera.ProcessMouseMovement(xoffset, yoffset);
    }

  } // namespace avion::core::common
