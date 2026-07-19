#include "AvionEngineCore/core/EventManager/EventListener.hpp"

#include "AvionEngineCore/renderer/camera.hpp"

namespace avion::core::eventmanager::detail
{
  CameraListener::CameraListener(void* observer)
  : m_camera(*(static_cast<gfx::Camera*>(observer)))
  {

  }

  void CameraListener::Update(int action, float value) 
  {
    
  }
} // namespace avion::core::eventmanager::detail