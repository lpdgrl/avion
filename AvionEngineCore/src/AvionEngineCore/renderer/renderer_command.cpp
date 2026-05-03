#include "AvionEngineCore/renderer/renderer_command.hpp"


namespace avion::gfx
{
  bool RendererCommand::InsertFbo(std::string fbo_name, float width, float height)
  {
    auto [_, result] = m_fbo_cache.try_emplace(fbo_name, width, height);

    return result;
  } 

  const FrameBuffer* RendererCommand::GetFbo(const std::string& fbo_name) const 
  {
    auto iter = m_fbo_cache.find(fbo_name);
    if (iter == m_fbo_cache.cend())
    {
      return nullptr;
    }
    
    return &iter->second;
   
  }
} // namespace avion::gfx