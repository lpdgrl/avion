#ifndef AVION_CORE_PROFILER_H 
#define AVION_CORE_PROFILER_H

  #include "glm/glm.hpp"

  namespace avion::core
  {
    struct CursorPosition 
    {
      double x_px_cursor  = 0;
      double y_px_cursor  = 0;
      double x_ndc_cursor = 0;
      double y_ndc_cursor = 0;
    };

    struct FrameState
    {
      int     m_frames;
      double  m_delay;
      double  m_lt;
    };

    struct CameraState
    {
      glm::vec3 camera_position;
    };

    struct Profiler 
    {
      FrameState frame_state;
      CursorPosition cursor_pos;
      CameraState camera_state;
    };
  } // namespace avion::core

#endif