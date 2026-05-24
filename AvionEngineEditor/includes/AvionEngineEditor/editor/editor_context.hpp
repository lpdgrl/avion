#ifndef AVION_EDITOR_EDITOR_CONTEXT_H
#define AVION_EDITOR_EDITOR_CONTEXT_H

#include "AvionEngineCore/core/engine.hpp"

namespace avion::editor::detail
{
  struct SelectPrimitive 
  {
    bool is_select = false;
    std::uint8_t id{};

    explicit operator bool() const { return is_select; }
  };

  struct SelectLight 
  {
    bool is_select = false;
    std::uint8_t id{};

    explicit operator bool() const { return is_select; }
  };

  struct SelectModel
  {
    bool is_select = false;
    std::uint16_t id{};
    std::string filename;

    explicit operator bool() const { return is_select; }
  };

  struct SelectionContext
  {
    SelectPrimitive primitive;
    SelectLight     light;
    SelectModel     model;
  };

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
    SelectionContext selection_ctx;
  };


  struct ProfilerContext
  {
    core::Profiler& profiler;
  };


} // namespace avion::editor::detail

#endif

