#ifndef AVION_GFX_RENDERER_COMMAND_H 
#define AVION_GFX_RENDERER_COMMAND_H

  #include <map>
  #include <string>

  #include "framebuffer.hpp"

  namespace avion::gfx
  {

    enum class RenderCommandType 
    {
      kCmdFbo = 1,
    };

    class RendererCommand
    {
      using FBOCache  = std::map<std::string, FrameBuffer>;
      public:
        RendererCommand() = default;

        bool InsertFbo(std::string fbo_name, float width, float height);
        const FrameBuffer* GetFbo(const std::string& fbo_name) const;
    
      private:
        FBOCache m_fbo_cache;
    };

  } // namespace avion::gfx

#endif