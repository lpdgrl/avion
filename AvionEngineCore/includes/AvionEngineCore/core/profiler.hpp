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
      glm::vec3 front_position;
      glm::vec3 up_position;
      glm::vec3 right_position;
      glm::vec3 world_up_position;

      float angle_yaw{};
      float angle_pitch{};

      float movement_speed{};
      float mouse_senstivity{};
      float zoom{};
    };

    struct RenderStatistics
    {
      static constexpr size_t kNumVertInTriang = 3;
      size_t draw_calls;
      size_t number_vertex;
      size_t number_indices;
      size_t number_triangles;

      inline void Clear() 
      {
        draw_calls        = 0;
        number_vertex     = 0;
        number_indices    = 0;
        number_triangles  = 0;
      }
   
      inline void Update(size_t n_vertex, size_t n_indices, size_t d_calls)
      {
        draw_calls += d_calls;
        number_vertex    += n_vertex;
        number_indices   += n_indices;
        number_triangles += n_vertex / kNumVertInTriang;
      }
    };

    struct RenderState
    {
      CameraState camera_state;
      RenderStatistics render_stat;
    };

    struct Profiler 
    {
      FrameState frame_state;
      CursorPosition cursor_pos;
      RenderState render_state;
    };
  } // namespace avion::core

#endif
