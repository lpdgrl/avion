#ifndef AVION_EDITOR_EDITOR_CONTEXT_H
#define AVION_EDITOR_EDITOR_CONTEXT_H

#include "AvionEngineCore/core/engine.hpp"

namespace avion::editor::detail
{

  struct EditorService
  {
    // core::Profiler& profiler;
  };

  struct EditorState
  {
    unsigned int texture_id_fbo = 0;
    unsigned int viewport_width = 0;
    unsigned int viewport_height = 0;
  };

  struct EditorContext
  {
    core::engine::Engine& engine;
    EditorState state;
  };


  struct ProfilerContext
  {
    core::Profiler& profiler;
  };


} // namespace avion::editor::detail

#endif

