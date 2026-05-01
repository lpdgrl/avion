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

  };

  struct EditorContext
  {
    core::engine::Engine& engine;
  };


  struct ProfilerContext
  {
    core::Profiler& profiler;
  };


} // namespace avion::editor::detail

#endif

