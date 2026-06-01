#include "AvionEngineCore/api/backend/backend.hpp"

namespace avion::api::backend
{
  Backend::Backend(RenderAPI api, RenderState init_state)
  : m_render_state(init_state)
  , m_api(api)
  {

  }

  void Backend::BeginFrame()
  {

  }

  void Backend::EndFrame()
  {

  }

  void Backend::UpdateFrame()
  {

  }

  void Backend::ApplyDrawCommand()
  {

  }

  void Backend::ApplyState()
  {

  }

  std::string Backend::GetApiString() const noexcept
  {
    std::string api;
    switch (m_api)
    {
      case RenderAPI::kOpengl:
      {
        api.append("Opengl");
        break;
      }
      case RenderAPI::kDirectX:
      {
        api.append("Directx");
        break;
      }
      case RenderAPI::kVulkan:
      {
        api.append("Vulkan");
        break;
      }
      case RenderAPI::kUnknown:
      {
        api.append("Unknown");
        break;
      }
    }
    return api;
  }
} // namespace avion::api::backend